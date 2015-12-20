#include "Manager.h"

#define DEBUG val->msg4="move from "+std::to_string(_move.first)+" to "+std::to_string(_move.second);

Manager* Manager::m_instance = 0;

Manager::Manager(void)
{
	

	bIsWhiteTurn = true;
	bSwapDice = false;
	bCurrentDiceDubel = false;
	isDiceLegal = false;
	_move.first = 1; _move.second =1;
	_dice.first = -2; _dice.second =-2;
	currentDice = &_dice.first;
	sDice = " ";
	turns = 0;
	//currentDice = 0;
	b = Board::instance();
	pWhite= new Player(bIsWhiteTurn); 
//system("pause"); //giving a differnt seed 
	pBlack= new Player(!bIsWhiteTurn); 
	currentPlayer = pWhite;
	val=Validator::instance();

	games = HOWMANYGAMESINAROW;
	mudul = MUDUL;
}

Manager* Manager::instance()
{
	if (!m_instance)
          m_instance = new Manager;
    return m_instance;
}

void Manager::ResetInstance()
{
    delete m_instance; // REM : it works even if the pointer is NULL (does nothing then)
    m_instance = NULL; // so GetInstance will still work.
}

Manager::~Manager(void)
{
	delete(pWhite->root);
	delete(pBlack->root);
	delete(pWhite->updateNode);
	delete(pBlack->updateNode);

	delete(pWhite);
	delete(pBlack);
}

void Manager::showHelp()
{
	cout << "\t\t RULES :" << endl<<endl;
	cout << "player 1 is White and playing the circles - O" << endl;
	cout << "player 2 is Black and playing the hashtags - #" << endl;
	cout << "at any point - you can type 'g' for giving up and lose the game" << endl;
	cout << "at a move input - you can type 's' for swapping the order of the dice" << endl;
	cout << "notice you have a pointer to current dice that need to be played - ^" << endl << endl;
	
	cout << "good luck" << endl << endl;

	system("pause");
	
}

void Manager::runGame()
{
	/* dont delete what u fix , just put V or X infront

	V	FIRST FIX : free memory
	V	computer swaping
		when looking on all moves , look at mdl pieces + canRemove(carful, the bool canremove not valid in those funcs)
		sacrifice dice : if tree doesnt have a node at first level : a.return after building a root(dont even try building second level). b.call a func that sacrifice dice.
		at the part where we return moves - check if its back and to tr[move].
		sDice bug: see which flow print the choose dice
		half move is all there is - do 1 level H()
	*/
	//swap com. mdlpcs+remove. h. sacrefice dice(if tree empty - thechoise ==null) , for black tr move . free memory
	initPointersOfMembers();
	//showHelp();
	
	//int j=0;
	//while(1)// limit of nodes ~7221000
	//{
	//	j++;
	//	if (  j>=7200000  )
	//	{
	//		cout << j << endl; 
	//		//break;
	//	}

	//	Node* a = new Node(b->a_board , pair<int,int> (5,5));

	//}

	/*bool dontpass = false;*/

	while(1)
	{
		/*if (dontpass)
		{*/
			sDice = "choose dice";	
			do //choose legal dice from bag
			{
				b->printBoard(val->msg1 , val->msg2 , val->msg3, val->msg4);
				_dice = currentPlayer->chooseDice();
				isDiceLegal = val->checkDice();
				currentDice = &_dice.first; 
			}
			while (!isDiceLegal);
			//int safty= 0;
			while (turns >0)
			{
				//safty++; if (safty >=5) swapDice();
				bool x = false;
				bCurrentDiceDubel ? x = !(val->dubelCannotMove()) : x = !(val->regCannotMove());
				if(x) playDice();
			} 
		/*}
		dontpass = true;*/
		
		sDice = "finished"; // refresh for next round
		//system("pause");
		b->turnPrintAllocationPrints = true;
		b->printBoard(val->msg1 , val->msg2 , val->msg3, val->msg4);
		currentPlayer->destroyLevel(currentPlayer->root);
		cout << "memoryTrace : " << Player::memoryTrace << endl;
		b->turnPrintAllocationPrints = false;
		b->n_round++;
		/*if(b->n_round  == 65)
		{
			system("pause");
		}
*/
		bIsWhiteTurn ? currentPlayer = pBlack:currentPlayer = pWhite ;
		bIsWhiteTurn = !bIsWhiteTurn;
		//system("pause");
 	}
}

void Manager::playDice()
{
	
	b->printBoard(val->msg1 , val->msg2 , val->msg3, val->msg4);
	do
	{
		if (bSwapDice) // fake swaping
		{
			bCurrentDiceDubel ?  val->msg1 = "swap is not an option" : swapDice();
			b->printBoard(val->msg1 , val->msg2 , val->msg3, val->msg4);
		}		
		_move = currentPlayer->chooseMove();
		/*if(currentPlayer->_isHuman == 0)
		{
			bool j = true;
 			system("pause");
		}*/

	}
	while(bSwapDice);
	
	if (val->checkMove())
	{
		turns--;
		b->emitBoard();
		DEBUG
		currentDice = &_dice.second;
	}
	if (b->a_out[0] >= 15 || b->a_out[1] >= 15)	
	{
		currentPlayer->destroyLevel(currentPlayer->root);
		gameOver("game over ",currentPlayer->s_color + " Wins !!! ");
	}
}

void Manager::gameOver(string msg1,string msg2 ,string msg3,string msg4)
{
	b->printBoard(msg1 , msg2 , msg3, msg4);
	system("pause");

	val->ResetInstance();
	b->ResetInstance();

	if(games--) 
	{
		mudul+=34;
		fakeDtor();
		fakeCtor();
		runGame();
	}



	this->~Manager();

	//b->printBoard();
	/*if(HOWMANYGAMESINAROW)
	{
		fakeDtor();
		fakeCtor();
		runGame();
	}*/
	/*fakeDtor();
	fakeCtor();

	runGame();*/
	
	exit(1);
}

void Manager::swapDice() // only swap on regular dice and when there r 2 dice to play
{
	if(turns==2 && (_dice.first != _dice.second))
	{
		int tmp    = _dice.first; _dice.first = _dice.second; _dice.second = tmp;
		sDice = std::to_string(_dice.first) + "x" + std::to_string(_dice.second);
		val->msg1 = "dice order swapped";
	}
}

void Manager::initPointersOfMembers() /// once ctor of manager called we have class of Player,Board,Validator and now we can set their pointers
{
	pWhite->initPointers();
	pBlack->initPointers();
	b->initPointers();
	val->initPointers();
	return;
}


void Manager::fakeDtor()
{
	delete(pWhite->root);
	delete(pBlack->root);
	delete(pWhite->updateNode);
	delete(pBlack->updateNode);

	delete(pWhite);
	delete(pBlack);

}

void Manager::fakeCtor()
{
	bIsWhiteTurn = true;
	bSwapDice = false;
	bCurrentDiceDubel = false;
	isDiceLegal = false;
	_move.first = 1; _move.second =1;
	_dice.first = -2; _dice.second =-2;
	currentDice = &_dice.first;
	sDice = " ";
	turns = 0;
	//currentDice = 0;
	b = Board::instance();
	pWhite= new Player(bIsWhiteTurn); 
//system("pause"); //giving a differnt seed 
	pBlack= new Player(!bIsWhiteTurn); 
	currentPlayer = pWhite;
	val=Validator::instance();

}