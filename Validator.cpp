#include "Validator.h"
 

Validator* Validator::v_instance = 0;

Validator* Validator::instance()
{
	if (!v_instance)
          v_instance = new Validator;
    return v_instance;
}

void Validator::ResetInstance()
{
    delete v_instance; // REM : it works even if the pointer is NULL (does nothing then)
    v_instance = NULL; // so GetInstance will still work.
}

Validator::Validator(void)
{

	man = NULL;
	aBoard = NULL;
	msg1 = " ";
	msg2 = " ";
	msg3 = " ";
	msg4 = " ";
	mdlPieces = 0;
	canRemovePieces = false;
	currentDice = 0;
	move.first =0;
	move.second =0;
	getDataUnecessary = true;
	doneCumputingMoves = true;
}

void Validator::initPointers()
{
	man = Manager::instance();
	aBoard = man->b->a_board;
	return;
}

Validator::~Validator(void)
{
}

void Validator::flushMsg()
{
	msg1 = " "; msg2 = " "; msg3 = msg4 ; msg4= " ";
}



//dice
bool Validator::checkDice()
{
	if((man->currentPlayer->_isHuman == 0) && PLAYINGAGAINSMAYANDROTEM  && (man->_dice.first == -5)) //player dont have any dice to play
	{
		man->turns = 0;
		msg1 = "Computer cant play anything";
		return true;
	}

	if (diceOutOfRange()) // check (1 <= dice <= 6)
	{
		msg1 = "dice need to be between 1 to 6";
		return false;
	}
	
	


	for (int i = 0; i < DICENUM ; i++)
	{
		if (man->currentPlayer->a_bag[i] == man->_dice  ) //entering here only if dice in bag
		{
			man->currentPlayer->a_bag[i].first = -1; //this how i mark dice not in bag
			man->currentPlayer->n_DiceInBag--; //total dice in bag
			string x("x");
			if (man->_dice.first == man->_dice.second) //double dice
			{
				string tmpDice(std::to_string(man->_dice.first));
				man->turns = 4;
				man->bCurrentDiceDubel = true;
				man->sDice = (tmpDice+x+tmpDice+x+tmpDice+x+tmpDice);
			}
			else //regular dice
			{
				man->sDice = std::to_string(man->_dice.first) + x + std::to_string(man->_dice.second) ;
				man->turns = 2;
				man->bCurrentDiceDubel = false;
			}
			return true;
		}
	}
	msg1 = "no such dice in the bag";
	if(man->currentPlayer->_isHuman == 0)
	{
		man->currentPlayer->doSwap = true;
	}
	return false;
}

bool Validator::diceOutOfRange()
{
	if ( man->_dice.first > 6 || man->_dice.first < 0 || man->_dice.second > 6 || man->_dice.second < 0)
	{
		return true;
	}
	return false;

}

//cannot move
void Validator::getPlayerData()
{
	bool isWhite = man->bIsWhiteTurn;
	if((man->currentPlayer->_isHuman == 1) || (doneCumputingMoves ))
	{
		isWhite ? aBoard = man->b->a_board : aBoard = man->b->a_rboard;
		mdlPieces = man->b->a_mdl[!isWhite];
	}
	else
	{
		aBoard = man->currentPlayer->updateNode->m_board;
		mdlPieces = man->currentPlayer->updateNode->mdlMe;
	}
	
	canRemovePieces = true;
	if (mdlPieces > 0 ) canRemovePieces = false;
	else
	{
		for (int i = 1; i < 19; i++)
		{
			if (aBoard[i] > 0 )  	{ canRemovePieces = false;	break; }
		}
	}	

	currentDice = man->currentDice;
}

bool Validator::dubelCannotMove()
{
	if (getDataUnecessary) 	getPlayerData(); 
	bool x = false;
	if (mdlPieces > 0)
	{
		x = dubelPlayerCannotComeBackFromMiddle(); 
	}
	else 
	{
		x = dubelNoRegularMove();
	}

	return x?true:false;
}

bool Validator::dubelPlayerCannotComeBackFromMiddle()
{
	if( aBoard[*currentDice] < -1) 
	{
		if (man->currentPlayer->_isHuman == 1)
		{
			msg1 = "cannot insert piece with " + man->sDice;
			msg2 = "turn is passed";
		}
		man->turns = 0;
		return true;
	}
	return false;
}

bool Validator::dubelNoRegularMove()
{
	bool a =true;
	if (canRemovePieces) a = canRemove(*currentDice);
	for (int i=1; i<24 && a==true ;i++)
	{
		if ( (aBoard[i] > 0) && (i + *currentDice <= 24) && (aBoard[*currentDice + i] > -2))
		{
			a = false;
		}
	}
	if (a) 
	{
		if (man->currentPlayer->_isHuman == 1)
		{
			msg1 = "cannot move any piece with " + man->sDice;
			msg2 = "turn is passed";
		}
		man->turns = 0;
	}
	return a;
}

bool Validator::regCannotMove()
{
	if (getDataUnecessary) getPlayerData();
	bool x = false;
	if (mdlPieces > 0)
	{
		x = regPlayerCannotComeBackFromMiddle(); 
	}
	else 
	{
		x = regNoRegularMove();
	}

	return x?true:false;
}



bool Validator::regPlayerCannotComeBackFromMiddle()
{
	if (aBoard[*currentDice] < -1) 
	{
		man->swapDice();
		if (aBoard[*currentDice] < -1) 
		{
			//error 
			man->swapDice();
			if (man->currentPlayer->_isHuman == 1)
			{
				msg1 = "cannot insert piece with " + man->sDice;
				msg2 = "turn is passed";
			}
			man->turns = 0;
			return true;
		}
	}
	return false;
}

bool Validator::regNoRegularMove()
{
	bool a =true;
	if (canRemovePieces) a = canRemove(*currentDice);
	for (int i=1; i<24 && a==true ;i++)
	{
		if ( (aBoard[i] > 0) && (i + *currentDice <= 24) && (aBoard[*currentDice + i] > -2))
		{
			a = false;
			break;
		}
	}

	if (a) 
	{
		if ( man->turns == 2) 
		{
			man->swapDice();
			if (canRemovePieces) a = canRemove(*currentDice);
			for (int i=1; i<24 && a==true ;i++)
			{
				if ( (aBoard[i] > 0) && (i + *currentDice <= 24) && (aBoard[*currentDice + i] > -2))
				{
					a = false;
					break;
				}
			}
		}
		if (a)
		{
			if (man->currentPlayer->_isHuman == 1)
			{
				msg1 = "cannot move any piece with " + man->sDice;
				msg2 = "turn is passed";
			}
			man->turns = 0;
		}
	}
	return a;
}



//move
bool Validator::checkMove()
{
	move = man->_move ;

	if (move.first == 0 && move.second==0)
	{
		msg1 = "move out of board";
		return false;
	}


	if(!(man->bIsWhiteTurn))
	{
		move.first = man->b->a_moveTransfer[move.first];
		if (move.second > 0 && move.second < 25)
		{
			move.second = man->b->a_moveTransfer[move.second];	
		}
		else move.second = 25;
	}
	


	if (mdlPieces)
	{
		if(piecesAreOnMdl())
		{ 
			if (man->currentPlayer->_isHuman == 0) man->currentPlayer->doSwapMove = true;
			return false;
		}
	}
	else
	{
		if (moveOutOfRange())
		{
			if (man->currentPlayer->_isHuman == 0) man->currentPlayer->doSwapMove = true;
			return false;
		}
		if(aBoard[move.first] <= 0)
		{ 
			if (man->currentPlayer->_isHuman == 0) man->currentPlayer->doSwapMove = true;
			msg1 = "you dont have any pieces there";
			return false;
		}
		if ( (move.first + *currentDice <= 24) && (aBoard[move.first + *currentDice] <= -2))
		{
			if (man->currentPlayer->_isHuman == 0) man->currentPlayer->doSwapMove = true;
			msg1 = "way is blocked";
			return false;
		}

	}
return true;
}

bool Validator::piecesAreOnMdl()
{
	if ( move.first > 6) //pieces on mdl but player didnt make a move to put them down	
	{
		msg1 = "must enter piece from the middle ";
		msg2 = "try again";
		return true;
	}
	if ( move.first != *currentDice) 
	{
		msg1 = "this is not your dice";
		msg2 = "try again";
		return true; // foul
	}
	if ( aBoard[move.first] <= -2 )
	{
		msg1 = "opponent pieces are on the way.";
		msg2 = "cannot enter there";
		msg3 = "try again";
		return true; // foul
	}
	
	
	return false;
}

bool Validator::moveOutOfRange()
{
	if(canRemovePieces && move.second > 24)
	{
		if (canRemove2(move.first, aBoard,*(man->currentDice))) {msg1 = "not leggal remove"; return true; }
	}
	else if (( move.first > 24 || move.first < 1)||( move.second > 24 || move.second < 1))
	{
		msg1 = "illegal move";
		return true;
	}
	

	return false;
}

bool Validator::canRemove(int x)
{
	int a =0;
	if (aBoard[25 - x] > 0) return false;
	for (int i =24 -x; i >= 19 && a==0; i--)
	{
		if (aBoard[i] > 0)
			a += aBoard[i];
	}
	if (a!=0) return true;
	return false;
}

bool Validator::canRemove2(int x , int* aBoard , int currentDice)
{
	int a =0;
	if ((aBoard[x] > 0) && (25-x== currentDice )) return false;
	for (int i =x-1; i >= 19 && a==0; i--)
	{
		if (aBoard[i] > 0)
			a += aBoard[i];
	}
	if (a!=0) return true;
	return false;
}










