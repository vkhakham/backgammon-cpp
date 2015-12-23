#include "Board.h"
#include "Manager.h"
using namespace std;


Board* Board::b_instance = 0;

Board::Board()
{
	n_round =1;
	turnPrintAllocationPrints = false;
	initBoard();
}

Board::~Board()
{
	
}

Board* Board::instance()
{
	if (!b_instance)
          b_instance = new Board;
    return b_instance;
}

void Board::ResetInstance()
{
    delete b_instance; // REM : it works even if the pointer is NULL (does nothing then)
    b_instance = NULL; // so GetInstance will still work.
}

void Board::initBoard()
{
	fill_n(a_board, 25, 0);
	fill_n(a_rboard, 25, 0);
	fill_n(a_moveTransfer, 25, 0);
	fill_n(a_mdl, 2, 0);
	fill_n(a_out, 2, 0);

	int num = 24;
	for (int i = 1; i < 25; i++)
	{
		a_moveTransfer[i] = num--; 
	}	
	
	a_board[1] = 2;
	a_board[6] = -5;
	a_board[8] = -3;
	a_board[12] = 5;
	a_board[13] = -5;
	a_board[17] = 3;
	a_board[19] = 5;
	a_board[24] = -2;


	//a_board[1] = -3;
	//a_board[2] = -1;
	//a_board[3] = -3;
	//a_board[4] = 3;
	//a_board[5] = 2;
	//a_board[6] = -3;
	//a_board[7] = 0;
	//a_board[8] = -1;
	//a_board[9] = 0;
	//a_board[10] = -1;
	//a_board[11] = 0;
	//a_board[12] = 0;
	//a_board[13] = 0;
	//a_board[14] = 0;
	//a_board[15] = -1;
	//a_board[16] = 0;
	//a_board[17] = -1;
	//a_board[18] = 0;
	//a_board[19] = 0;
	//a_board[20] = 0;
	//a_board[21] = 2;
	//a_board[22] = 2;
	//a_board[23] = 6;
	//a_board[24] = -1;
	//a_mdl[0] = 1;


	//a_board[1] = -2;
	//a_board[2] = -1;
	//a_board[3] = -3;
	//a_board[4] = -2;
	//a_board[5] = -3;
	//a_board[6] = -3;
	//a_board[7] = -1;
	//a_board[12] = 0;
	//a_board[14] = 1;
	//a_board[16] = 1;
	//a_board[18] = 0;
	////a_board[18] = -2;
	//a_board[19] = -2;
	//a_board[20] = -2;
	//a_board[21] = -2;
	//a_board[22] = -2;
	//a_board[23] = -2;
	//a_board[24] = -2;

	//a_mdl[0] = 1;

	for (int i = 0; i < 25; i++)
	{
		a_rboard[a_moveTransfer[i]] = a_board[i]* -1; 
	}





	/*a_rboard[1] = 2;
	a_rboard[6] = -5;
	a_rboard[8] = -3;
	a_rboard[12] = 5;
	a_rboard[13] = -5;
	a_rboard[17] = 3;
	a_rboard[19] = 5;
	a_rboard[24] = -2;*/




	
	/*a_mdl[0] = 2;
	a_mdl[1] = 0;
*/
	

//	a_mdl[1] = 1;
}




void Board::emitBoard()
{

	if (man->bIsWhiteTurn) //white turn
	{
		if( a_mdl[0] > 0 ) // player want to put down his mdl pieces
		{
			a_mdl[0]--;
			if (a_board[man->_move.first] == -1) // eat black piece
			{
				a_mdl[1]++;
				a_board[man->_move.first] = 0; 
				a_rboard[a_moveTransfer[man->_move.first]] = 0;
			}
			a_board[man->_move.first]++;
			a_rboard[a_moveTransfer[man->_move.first]]--;
		}
		else //regular move; checking if a piece is on the way
		{
			a_board[man->_move.first]--;
			a_rboard[a_moveTransfer[man->_move.first]]++;
			if(man->_move.second<=24)
			{
				if (a_board[man->_move.second] == -1) 
				{
					a_mdl[1]++; // put this black piece on mdl
					a_board[man->_move.second] = 0; //reset the slot to 0;
					a_rboard[a_moveTransfer[man->_move.second]] = 0; //#bug
				}
				a_board[man->_move.second]++;
				a_rboard[a_moveTransfer[man->_move.second]]--;
			}
			else
			{
				a_out[0]++;
			}
		}
	}
	else //black turn
	{
		if( a_mdl[1] > 0) // player want to put down his mdl pieces
		{
			a_mdl[1]--;
			if (a_board[man->_move.first] == 1) // eat white piece
			{
				a_mdl[0]++;
				a_board[man->_move.first] = 0; 
				a_rboard[a_moveTransfer[man->_move.first]] = 0; 
			}
			a_board[man->_move.first]--;
			a_rboard[a_moveTransfer[man->_move.first]]++;
		}
		else
		{
			a_board[man->_move.first]++;
			a_rboard[a_moveTransfer[man->_move.first]]--;
			if(man->_move.second >= 1)
			{
				if (a_board[man->_move.second] == 1) 
				{
					a_mdl[0]++; // put this white piece on mdl
					a_board[man->_move.second] = 0; //reset the slot to 0;
					a_rboard[a_moveTransfer[man->_move.second]] = 0;
				}
				a_board[man->_move.second]--;
				a_rboard[a_moveTransfer[man->_move.second]]++;
			}
			else
			{
				a_out[1]++;
			}
		}
	}
}

void Board::printBoard(string msg1  ,string msg2 ,string msg3  ,string msg4 )
{
	string strFirstRow("-01-02-03-04-05-06--MDL--07-08-09-10-11-12--OUT");
	string strLastRow ("-24-23-22-21-20-19--MDL--18-17-16-15-14-13--OUT");
	string buffer     ("============================================");
	int mdWhite = a_mdl[0];
	int mdBlack = a_mdl[1];
	int outWhite= a_out[0];
	int outBlack= a_out[1];
	int counterWhite = DICENUM_VERSION1;
	int counterBlack = DICENUM_VERSION1;

	cout <<endl << endl << endl<< "\t\t\tROUND " << n_round <<endl;
	cout << strFirstRow /*+ "   WHITE bag of dice:"*/ <<endl ;
	
	for (int i = 1; i <= 8; i++)
	{
		for (int j=1 ; j<= 12; j++)
		{
			if (j==7)
			{ 
				if ((mdBlack >8) && (mdBlack - 8 >=i)) 
					cout << " (## ) ";
				else if (mdBlack >=i)
					cout << " ( # ) ";
				else cout << " (   ) ";
			}
			else
			{
				cout << "|";
			}
			bool bIsWhite = true;
			int val = a_board[j];
			if (val < 0) 
			{
				bIsWhite = false;
				val = -val;
			}
			if ((val >8) && (val - 8 >=i)) 
				bIsWhite ? cout << "OO" : cout << "##";
			else if (val >= i) 
				bIsWhite ? cout << " O" : cout << " #";
			else cout << "  ";
		}
		cout << "|| ";
		if ((outBlack >8) && (outBlack - 8 >=i)) 
				cout << "##";
			else if (outBlack >= i) 
				cout << " #";
			else cout << "  ";
		cout << "\t";
		for (int h = 0; h < 6 && counterWhite > 0; h++)
		{
			if ( 0)//man->pWhite->a_bag[DICENUM_VERSION1-counterWhite].first != -1)
			{
					//cout <<" "  << man->pWhite->a_bag[DICENUM_VERSION1-counterWhite].first 
						//	  << "x" << man->pWhite->a_bag[DICENUM_VERSION1-counterWhite].second;
			}
			else
			{
				cout << "    ";
			}
			counterWhite--;
		}
		cout << endl;
	}
	cout << buffer << endl;// +"      BLACK bag of dice:" << endl;
	for (int i = 8; i >= 1; i--)
	{
		for (int j=24 ; j>= 13; j--)
		{
			if (j==18)
			{ 
				if ((mdWhite >8) && (mdWhite - 8 >=i)) 
					cout << " (OO ) ";
				else if (mdWhite >=i)
					cout << " ( O ) ";
				else cout << " (   ) ";
			}
			else
			{
				cout << "|";
			}
			bool bIsWhite = true;
			int val = a_board[j];
			if (val < 0) 
			{
				bIsWhite = false;
				val = -val;
			}
			if ((val >8) && (val - 8 >=i)) 
				bIsWhite ? cout << "OO" : cout << "##";
			else if (val >= i) 
				bIsWhite ? cout << " O" : cout << " #";
			else cout << "  ";
		}
		cout << "|| ";
		if ((outWhite >8) && (outWhite - 8 >=i)) 
				cout << "OO";
			else if (outWhite >= i) 
				cout << " O";
			else cout << "  ";
		cout << "\t";
		for (int h = 0; h < 6 && counterBlack > 0; h++)
		{
			if (0)// man->pBlack->a_bag[DICENUM_VERSION1-counterBlack].first != -1)
			{
				/*cout <<" "  << man->pBlack->a_bag[DICENUM_VERSION1-counterBlack].first
						  << "x" << man->pBlack->a_bag[DICENUM_VERSION1-counterBlack].second;*/
			}
			else
			{
				cout << "    ";
			}
			counterBlack--;
		}
		cout << endl;
	}
	string x("                ");
	if (man->bCurrentDiceDubel)
	{
		if (man->turns == 1)        x.append("      ^");
		else if (man->turns == 2)	x.append("    ^");
		else if (man->turns == 3)	x.append("  ^");
		else if (man->turns == 4)	x.append("^");
	}
	else 
	{
		if (man->turns == 2)		x.append("^");
		else if (man->turns == 1)	x.append("  ^");
	}
	if (man->turns == 0) x = " ";

	
	cout << strLastRow << endl <<endl;
	cout << buffer << " white out: "<< a_out[0] << " canRemovePieces: " << Validator::instance()->canRemovePieces <<endl ;
	cout << "\t" << man->currentPlayer->s_color + "'s move:   "  << man->sDice <<endl ;
	cout << "\t" << x << endl;
	cout << ">>>" << msg1 << endl ;
	cout << ">>>" << msg2 << endl ;
	cout << ">>>" << msg3 << endl ;
	cout << ">>>" << msg4 << endl ;
	cout << buffer << " black out: " << a_out[1] << "      dice: "<< man->_dice.first <<"x"<< man->_dice.second<<endl;

	if (man->dice_version == 1)
	{
		cout << "WHITE bag of dice:" << endl;
		for (int i = 1; i < 7; i++)
		{
			for (int j = 1; j < 7; j++)
			{
				if (man->pWhite->m_bag[i][j] > 0)
				{
					int x = man->pWhite->m_bag[i][j];
					for (int k = 0; k < x; k++)
					{
						cout << i << "x" << j << " ";
					}
				}
				else if (man->pWhite->m_bag[i][j] == 0)
				{
					cout << "    ";
				}
			}
		}
		cout <<endl<< "Black bag of dice:" << endl;
		for (int i = 1; i < 7; i++)
		{
			for (int j = 1; j < 7; j++)
			{
				if (man->pBlack->m_bag[i][j] > 0)
				{
					int x = man->pBlack->m_bag[i][j];
					for (int k = 0; k < x; k++)
					{
						cout << i << "x" << j << " ";
					}
				}
			}
		}
		cout << endl;
	}
	else
	{ 
		cout << "SHARED bag of dice:" << endl;
		for (int i = 1; i < 7; i++)
		{
			for (int j = 1; j < 7; j++)
			{
				if (man->pWhite->m_bag[i][j] > 0)
				{
					int x = man->pWhite->m_bag[i][j];
					for (int k = 0; k < x; k++)
					{
						cout << i << "x" << j << " ";
					}
				}
				else if (man->pWhite->m_bag[i][j] == 0)
				{
					cout << "    ";
				}
			}
		}
		cout << endl;
	}



	if (turnPrintAllocationPrints)
	{
		cout << "first level: "<< man->currentPlayer->player_root->children.size()<<endl;
		cout << "second level: "<< man->currentPlayer->allTreeNodes << endl;
	}

	man->val->flushMsg();
}

void Board::initPointers()
{
	man = Manager::instance();
	return;
}
