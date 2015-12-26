#include "Manager.h"



Manager* Manager::m_instance = 0;

Manager::Manager(void)
{
	cout << "Please choose game version: " << endl;
	cout << "1 = Each player has an independent bag of 46 dices " << endl;
	cout << "2 = Both player share a bag of 72 dices " << endl;
	//cin >> dice_version;
	cout << "Please choose minimax version: (0 = minMax, 1 = minMax Alpha Beta)" << endl;
	//cin >> alpha_beta;
	cout << "Please enter max depth of minmax: (bigger than 1) " << endl;
	//cin >> maxDepth;
	cout << "What is the type of Player 1 ?: " << endl;
	cout << "0 = AI " << endl;
	cout << "1 = Human " << endl;
	//cin >> player1;
	cout << "What is the type of Player 2 ?: " << endl;
	cout << "0 = AI " << endl;
	cout << "1 = Human " << endl;
	//cin >> player2;
	cin.clear();
	fflush(stdin);
	dice_version = 2;
	alpha_beta = 1;
	maxDepth = 3;
	player2 = 0;
	player1 = 0;
	bIsWhiteTurn = true;
	bSwapDice = false;
	bCurrentDiceDubel = false;
	isDiceLegal = false;
	_move.first = 1; _move.second =1;
	_dice.first = -2; _dice.second =-2;
	currentDice = &_dice.first;
	sDice = " ";
	turns = 0;
	b = Board::instance();
	pWhite = new Player(bIsWhiteTurn, dice_version, player1);
	system("pause");
	pBlack = new Player(!bIsWhiteTurn, dice_version, player2);
	currentPlayer = pWhite;
	val=Validator::instance();

	games = HOWMANYGAMESINAROW;
	strategy_ptr = new strategy();
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
	delete(pWhite->player_root);
	delete(pBlack->player_root);
	delete(pWhite->updateNode);
	delete(pBlack->updateNode);

	delete(pWhite);
	delete(pBlack);
}

void Manager::showHelp()
{
	cout << "\t\t RULES :" << endl<<endl;
	cout << "player 1 is White and playing the circles - O" << endl;
	cout << "player 2 is Black and playing the hash tags - #" << endl;
	cout << "at any point - you can type 'g' for giving up and lose the game" << endl;
	cout << "at a move input - you can type 's' for swapping the order of the dice" << endl;
	cout << "notice you have a pointer to current dice that need to be played - ^" << endl << endl;
	
	cout << "good luck" << endl << endl;

	system("pause");
	
}

void Manager::runGame()
{
	initPointersOfMembers();
	//showHelp();
	while(1)
	{
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
		sDice = "finished"; // refresh for next round
		b->turnPrintAllocationPrints = true;
		b->printBoard(val->msg1 , val->msg2 , val->msg3, val->msg4);
		currentPlayer->destroyLevel(currentPlayer->player_root);
		cout << "memoryTrace : " << Player::memoryTrace << endl;
		b->turnPrintAllocationPrints = false;
		b->n_round++;
		
	/*	if(b->n_round  == 13)
		{
			system("pause");
		}*/
		
		bIsWhiteTurn ? currentPlayer = pBlack:currentPlayer = pWhite ;
		bIsWhiteTurn = !bIsWhiteTurn;
 	}
}

void Manager::playDice()
{
	
	b->printBoard(val->msg1 , val->msg2 , val->msg3, val->msg4);
	do
	{
		if (bSwapDice) // fake swapping
		{
			bCurrentDiceDubel ?  val->msg1 = "swap is not an option" : swapDice();
			b->printBoard(val->msg1 , val->msg2 , val->msg3, val->msg4);
		}		
		_move = currentPlayer->chooseMove();
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
		currentPlayer->destroyLevel(currentPlayer->player_root);
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
		runGame();
	this->~Manager();
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
	//else if ((currentPlayer->_isHuman == 0) && (currentPlayer->theChoise->m_move2.first > 0))
	//{
	//	int tmp = _dice.first; _dice.first = _dice.second; _dice.second = tmp;
	//	sDice = std::to_string(_dice.first) + "x" + std::to_string(_dice.second);
	//	val->msg1 = "dice order swapped";
	//}

}

void Manager::initPointersOfMembers() /// once ctor of manager called we have class of Player,Board,Validator and now we can set their pointers
{
	pWhite->initPointers();
	pBlack->initPointers();
	b->initPointers();
	val->initPointers();
	return;
}
