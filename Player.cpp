#include "Player.h"
#include "Manager.h"


#define PLAYER1 0
#define PLAYER2 0
#define TREELEVELS 2
//#define MUDUL 555


int Player::memoryTrace = 0;

Player::Player(bool p)
{
	round = 0;
	int f = 0;
	turn = -1;
	isWhitePlayer = p;
	p ? _isHuman = PLAYER1 : _isHuman = PLAYER2;
	p ? s_color="WHITE" : s_color = "BLACK"; 
	n_DiceInBag = DICENUM;
//n_DiceInBag = 1;
	for (int i = 1; i <= 6; i++)
	{
		for (int j = 1; j <= 6; j++)
		{
			a_bag[f].first = i ;
			a_bag[f++].second = j;
		}
	}
	srand ((unsigned int)time(NULL));
	for (int i = f; i < f+10; i++)
	{
		a_bag[i].first = 6;// rand() % 6 + 1;
		a_bag[i].second = 6;//rand() % 6 + 1;
	}
	man = NULL;

	//pair<int,int> p1(-1,6);
	//pair<int,int> p2(6,6);
	//p ? a_bag[36] = p1 : a_bag[36] = p2;
	/*a_bag[36] = pair<int,int> (5,4);
	a_bag[37] = pair<int,int> (6,1);
	a_bag[38] = pair<int,int> (3,4);
	a_bag[39] = pair<int,int> (3,1);
	a_bag[40] = pair<int,int> (1,5);
	a_bag[41] = pair<int,int> (4,4);
	a_bag[42] = pair<int,int> (2,1);
	a_bag[43] = pair<int,int> (2,2);
	a_bag[44] = pair<int,int> (5,4);
	a_bag[45] = pair<int,int> (2,5);*/
	//a_bag[45] = pair<int, int>(2, 5);
	/*if (!p )
	{
		n_DiceInBag = 8;
		a_bag[17] = pair<int,int> (6,6);
		a_bag[19] = pair<int,int> (6 ,6);
		a_bag[23] = pair<int,int> (6,6);
		a_bag[29] = pair<int,int> (6,6);
		a_bag[30] = pair<int,int> (6,6);
		a_bag[32] = pair<int,int> (6,6);
		a_bag[39] = pair<int,int> (6,6);
		a_bag[45] = pair<int,int> (6,6);
	}
	if (p )
	{
		n_DiceInBag = 7;
		a_bag[3] = pair<int,int> (6,6);
		a_bag[17] = pair<int,int> (6,6);
		a_bag[30] = pair<int,int> (6,6);
		a_bag[36] = pair<int,int> (6,6);
		a_bag[37] = pair<int,int> (6,6);
		a_bag[41] = pair<int,int> (6,6);
		a_bag[45] = pair<int,int> (6,6);
	

	}*/
	

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			a_remainingDice[i][j] = 0;
			bagForLevel[i][j] = 0;
	        a_remainingDiceOpponent[i][j] = 0;
		}
	}
	fill_n(a_moveTransfer, 25, 0);
	int num = 24;
	for (int i = 1; i < 25; i++)
	{
		a_moveTransfer[i] = num--; 
	}
	doSwap = false;
	doSwapMove = false;
	stucked = false;
	theChoise = NULL;
	root = new Node(boardForLevel , 0 , 0, pair<int,int>(-1,-1));
	updateNode = new Node(boardForLevel , 0 , 0, pair<int,int>(-1,-1));
	lastTry = -1;
	allTreeNodes = -1;

}

Player::~Player(void)
{
}

pair<int,int> Player::chooseDice()
{
	pair<int,int> p(-5,-5);
	string input1 = " ";
	string input2 = " ";
	/*cin.clear();
	fflush(stdin);*/
	if (n_DiceInBag == 0)
	{
		man->gameOver("game over . no more dice to play","its a Tie !!! " );
	}
	if (_isHuman) //human
	{
		cout << "first  dice: ";
		cin >> input1;	
		if (input1 == "g")
		{
			string a;
			man->bIsWhiteTurn ? a = "BLACK Wins !!!" : a = "WHITE Wins !!!";
			man->gameOver("game over", man->currentPlayer->s_color + " gives up the game" , a);
		}
		cout << "second dice: ";
		cin >> input2;
		if (input2 == "g")
		{
			string a;
			man->bIsWhiteTurn ? a = "BLACK Wins !!!" : a = "WHITE Wins !!!";
			man->gameOver("game over", man->currentPlayer->s_color + " gives up the game" , a);
		}
		p.first  = atoi(input1.c_str());
		p.second = atoi(input2.c_str());
		if(p.first > 6 || p.first < 1 || p.second > 6 || p.second < 1) 
			return p;
	}
	else //computer
	{
		//p = chooseFirstDiceFromBag();
		man->val->doneCumputingMoves = false;
		if (doSwap == false) 
			theChoise = chooseWhatToDo();
		
		if(theChoise)
		{
			if (doSwap)
			{
				p.first = theChoise->m_dice.second;
				p.second= theChoise->m_dice.first;
				doSwap = false;
			}
			else
			{
				p = theChoise->m_dice;
			}
		}
		else // the choise == null - no children to root.
		{
			isWhitePlayer ? theChoise = halfMoveChooseWhatToDoWhiteStart() : theChoise = halfMoveChooseWhatToDoBlackStart(); 
			if (theChoise )
			{
				if (doSwap)
				{
					p.first = theChoise->m_dice.second;
					p.second= theChoise->m_dice.first;
					doSwap = false;
				}
				else
				{
					p = theChoise->m_dice;
				}
			}
			else// the choise == null
			{
				if (! PLAYINGAGAINSMAYANDROTEM)  p = sacrificeADice(); 
			}
		}
		

	}
	man->val->doneCumputingMoves = true;
	return p; //p == -5,-5  default
}

pair<int,int> Player::chooseMove()
{
	string input;
	pair<int,int> p(0,0);
	/*cin.clear();
	fflush(stdin);*/
	if (_isHuman) //human
	{
		cout << "from which column: ";
		cin >> input;
		if (input != "s" && input != "g")
		{
			p.first = atoi(input.c_str());
			if(p.first <= 24 && p.first >= 1)
			{
				if (man->bIsWhiteTurn) p.second = p.first + *man->currentDice;	
				else p.second = p.first - *man->currentDice;
			}	
			else p.first = 0;
			man->bSwapDice = false;
		}
		else if (input == "s")
		{
			man->bSwapDice = true;
		}
		else
		{
			string a;
			man->bIsWhiteTurn ? a = "BLACK Wins !!!" : a = "WHITE Wins !!!";
			man->gameOver("game over", man->currentPlayer->s_color + " gives up the game" , a);
		}
	}
	else //computer
	{
		bool isWhite = man->bIsWhiteTurn; // to return the move if(blackTurn) we'll put it in a_transefer
		isWhite ? man->val->aBoard = man->b->a_board : man->val->aBoard = man->b->a_rboard; //validator think there is only white checks. so we'll give him the "right" board just for checks
		man->val->mdlPieces = man->b->a_mdl[!(isWhite)]; //get hom much pieces on mdl. white is in index 0 and black 1
	
		if (theChoise) //should never be null here! . if null => we dont have full move or half move . should have not come here
		{
			if (theChoise->m_dice.first == theChoise->m_dice.second) // double dice
			{
				if (man->turns == 4)
				{
					if(isWhite)  p = theChoise->m_move1 ;
					else 
					{
						p.first = a_moveTransfer[theChoise->m_move1.first];
						if (theChoise->m_move1.second <=24) p.second= a_moveTransfer[theChoise->m_move1.second];
						else p.second = 0; //if 0 that me p.second is "remove piece"
					}
				}
				if (man->turns == 3)
				{
					if(isWhite) p = theChoise->m_move2;
					else
					{
						p.first = a_moveTransfer[theChoise->m_move2.first];
						if (theChoise->m_move2.second <=24) p.second= a_moveTransfer[theChoise->m_move2.second];
						else p.second = 0;
					}
				}
				if (man->turns == 2)
				{
					if(isWhite) p = theChoise->m_move3;
					else
					{
						p.first = a_moveTransfer[theChoise->m_move3.first];
						if (theChoise->m_move3.second <=24) p.second= a_moveTransfer[theChoise->m_move3.second];
						else p.second = 0;
					} 
				}	
				if (man->turns == 1)  
				{
					if(isWhite) p = theChoise->m_move4;
					else
					{
						p.first = a_moveTransfer[theChoise->m_move4.first];
						if (theChoise->m_move4.second <=24) p.second= a_moveTransfer[theChoise->m_move4.second];
						else p.second = 0;
					} 
				}
			}
			else //reg dice
			{
				if (doSwapMove && man->turns == 2) //just safty net . if computer somehow did wrong move swap dice only if turns == 2
				{
					man->swapDice();
					doSwapMove = false;
				}

				//if (man->turns == 2) // we know the first move is correct 100%. all we need to see if we need to tell manager to swap the dice
				//{
				//	if(theChoise->whatMoveAmI1 == "remove piece" && (*(man->currentDice) != 25 ) // if its remove - than move1.second should be 25
				//}

				if(man->_dice != theChoise->m_dice) 
				{
					man->swapDice(); 
				}



				//if( theChoise->m_move1!="remove piece" && theChoise->m_move1!="going Down" && (*(man->currentDice) != theChoise->m_move1.second - theChoise->m_move1.first )  && (man->turns == 2)) //the dice is pointed backwards
				//{//this could happen all the time. example : in bag there is 2x1 and computer calculated 1x2
				//	man->swapDice();
				//}
				
				if(man->turns == 2)
				{
					if(isWhite) p = theChoise->m_move1;	
					else
					{
						p.first = a_moveTransfer[theChoise->m_move1.first];
						if (theChoise->m_move1.second <=24) p.second= a_moveTransfer[theChoise->m_move1.second];
						else p.second = 0;
					} 
				}
				else //second move 
				{//in half move : validator should make sure to put in turns 1 and not come here
					if(isWhite) p = theChoise->m_move2;	
					else
					{
						p.first = a_moveTransfer[theChoise->m_move2.first];
						if (theChoise->m_move2.second <=24) p.second= a_moveTransfer[theChoise->m_move2.second];
						else p.second = 0;
						//p.second= a_moveTransfer[theChoise->m_move2.second];
					} 
					
				}
			}
		}
		
	}
	
	return p;
}

Node* Player::chooseWhatToDo()
{
	delete(root);
	root = NULL;
	bool is_white = man->bIsWhiteTurn;
	allTreeNodes = 1;
	getRemainingDice(a_bag, is_white ? man->pWhite->a_bag : man->pBlack->a_bag);
	for (int i = 0; i < 25; i++) { boardForLevel[i] = is_white ? man->b->a_rboard[i]: man->b->a_board[i]; }
	// a_mdl[0] is white middle
	root = new Node(boardForLevel, man->b->a_mdl[is_white], man->b->a_mdl[!is_white], pair<int, int>(-1, -1));
	root->whiteLevel = !is_white;
	build_tree(TREELEVELS);

	return root->children.size() > 0 ? root->children.at(mudul % (root->children.size())) : NULL;
}

void Player::build_tree(int depth)
{
	buildOneLevel(root);
	if (depth > 1)
	{
		swapBagForDiffrentLevel(1);
		for (Node* child : root->children)
		{
			a_remainingDice[child->m_dice.first][child->m_dice.second]--;
			buildOneLevel(child);
			a_remainingDice[child->m_dice.first][child->m_dice.second]++;

			//delete(child);
		}
	}
}

void Player::initPointers()
{
	man = Manager::instance();
	mudul= man->mudul;
	return;
}

void Player::buildOneLevel(Node* levelFather)
{
	// reverse the fathers board and middles and bags?
	for (int i = 0; i < 25; i++)
	{
		boardForLevel[a_moveTransfer[i]] = levelFather->m_board[i]* (-1); 
		updateNode->m_board[a_moveTransfer[i]] = levelFather->m_board[i]* (-1); 
	}
	updateNode->mdlMe = levelFather->mdlHim;
	
	for (int i = 1; i < 7; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (bagForLevel[i][j] > 0) 
			{
				man->_dice = pair<int,int>(i,j);	
				bool x = false;
				bool isDubel = (i == j);
				
				if(isDubel)
				{
					man->turns = 4;
					x = !(man->val->dubelCannotMove());//player data !
					if (x)
					{	
						man->turns = 4;
						searchDoubleMove(levelFather, i);
					}
				}
				else
				{
					man->turns = 2;
					x = !(man->val->regCannotMove());
					if (x)
					{	
						man->turns = 2;
						searchRegMove(levelFather, pair<int, int>(i, j));
					}
				}
				man->val->getDataUnecessary = false;
			} 
		}
	}
	man->val->getDataUnecessary = true;
}

void Player::searchRegMove(Node *levelFather, pair<int,int>& tempDice)
{
	int idDice=0;
	int tempBoard[25];
	bool pin1 = false;
	bool pin2 = false;
	bool ate1 = false;
	bool ate2 = false;
	stucked=false;
	int tempMdlme  = levelFather->mdlHim;
	int tempMdlhim = levelFather->mdlMe;
	stucked=false;

	memcpy(tempBoard,boardForLevel, sizeof(int) * 25);
	for (int k = 0; k < 2; k++)
	{
		if (k==1)
		{
			pin1 = pin2 = false;
			stucked=false;
			int swap = tempDice.second; 
			tempDice.second =tempDice.first;
			tempDice.first = swap;
		}
		for (int i = 1; i < 25 && pin1==false; i++)
		{
			if ( int rev1 = chekcDoubleEntry(tempBoard,  &tempMdlme, &tempMdlhim,  i,  tempDice.first, &pin1 , &ate1) )
			{	
				if(stucked) { break;}
				for (int j = 1; j < 25 && pin2==false ; j++)
				{	
					if ( int rev2 = chekcDoubleEntry(tempBoard,  &tempMdlme, &tempMdlhim,  j,  tempDice.second, &pin2 , &ate2) )
					{
						if(stucked) { break;}
						pair<int,int> move1;
						pair<int,int> move2;
						int temp ;
						pin1 ? temp = tempDice.first : temp = i;
						move1.first = temp;
						move1.second = temp + tempDice.first ;
						if ( move1.second > 24) { move1.second = 25;}
						else if (pin1)			  { move1.second = move1.first;}
						pin2 ? temp = tempDice.second : temp = j;
						move2.first  = temp;
						move2.second = temp + tempDice.second ;
						if ( move2.second > 24) { move2.second = 25;}
						else if (pin2)			  { move2.second = move2.first;}
						bool c = true;
						int y = levelFather->children.size();
						for (int f =y-idDice ; f <y ; f++) //checking to see if this node already been made
						{
							if ((levelFather->children[f]->m_move1 == move1 && levelFather->children[f]->m_move2 == move2  ) 
								|| (levelFather->children[f]->m_move2 == move1 && levelFather->children[f]->m_move1 == move2))
							{
								c = false;
								break;
							}
						}
						if(c)
						{
							Node* myNode = new Node(tempBoard,tempMdlme,tempMdlhim,tempDice);
							Player::memoryTrace++;
							myNode->whiteLevel = !(levelFather->whiteLevel);
							allTreeNodes++;					
							myNode->m_move1= move1;
							
							myNode->m_move2 = move2;
							

							myNode->idDice = ++idDice;
							levelFather->children.push_back(myNode); 
						}
						revDubleChanges(rev2 , &ate2 , tempBoard , j, &tempMdlme , &tempMdlhim , tempDice.second);
					}
				} 
				revDubleChanges(rev1 , &ate1 , tempBoard , i, &tempMdlme , &tempMdlhim , tempDice.first);
			}
		} 
	}
}

void Player::searchDoubleMove(Node *levelFather,int& tempDice )
{
	bool pin1,pin2,pin3,pin4;
	bool ate1,ate2,ate3,ate4;
	pin1=pin2=pin3=pin4=false;
	ate1=ate2=ate3=ate4=false;
	int idDice=0;
	int tempBoard[25];
	int tempMdlme  = levelFather->mdlHim;
	int tempMdlhim = levelFather->mdlMe;

	memcpy(tempBoard,boardForLevel, sizeof(int) * 25);

	for (int i = 1; i < 25 && pin1==false; i++)
	{
		if(int rev1 = chekcDoubleEntry(tempBoard,&tempMdlme , &tempMdlhim , i ,tempDice , &pin1 ,&ate1) )
		{
			for (int h = i; h < 25 && pin2==false; h++)
			{
				if(int rev2 = chekcDoubleEntry(tempBoard,&tempMdlme , &tempMdlhim , h ,tempDice , &pin2 , &ate2) )
				{
					for (int k = h; k < 25 && pin3==false; k++)
					{
						if(int rev3 = chekcDoubleEntry(tempBoard,&tempMdlme , &tempMdlhim , k ,tempDice , &pin3 , &ate3) )
						{
							for (int j = k; j < 25 && pin4==false; j++)
							{	
								if(int rev4 = chekcDoubleEntry(tempBoard,&tempMdlme , &tempMdlhim , j ,tempDice , &pin4 , &ate4) )
								{
									Node* myNode = new Node(tempBoard,tempMdlme,tempMdlhim,pair<int,int>(tempDice,tempDice));
									Player::memoryTrace++;
									int temp;
									allTreeNodes++;
									myNode->whiteLevel = !(levelFather->whiteLevel);
									pin1 ? temp = tempDice : temp =i;
									myNode->m_move1.first  = temp; 
									myNode->m_move1.second = temp + tempDice;
									if ( myNode->m_move1.second > 24) {  myNode->m_move1.second = 25;}
									else if (pin1)					  {  myNode->m_move1.second = myNode->m_move1.first;}
									pin2 ? temp = tempDice : temp =h;
									myNode->m_move2.first  = temp; 
									myNode->m_move2.second = temp + tempDice; 
									if ( myNode->m_move2.second > 24) { myNode->m_move2.second = 25;}
									else if (pin2)					  { myNode->m_move2.second = myNode->m_move2.first;}
									pin3? temp = tempDice : temp = k;
									myNode->m_move3.first  = temp; 
									myNode->m_move3.second = temp + tempDice; 
									if ( myNode->m_move3.second > 24) {  myNode->m_move3.second = 25;}
									else if (pin3)					  {  myNode->m_move3.second = myNode->m_move3.first;}
									pin4? temp = tempDice : temp = j;
									myNode->m_move4.first = temp;
									myNode->m_move4.second= temp +  tempDice;
									if ( myNode->m_move4.second > 24) { myNode->m_move4.second = 25;}
									else if (pin4)					  { myNode->m_move4.second = myNode->m_move4.first;}
									myNode->idDice = ++idDice;
									levelFather->children.push_back(myNode);
									revDubleChanges(rev4 , &ate4 , tempBoard, j, &tempMdlme , &tempMdlhim , tempDice);
								}
							} 
							revDubleChanges(rev3 , &ate3, tempBoard, k, &tempMdlme , &tempMdlhim , tempDice);
						}	
					} 
					revDubleChanges(rev2 , &ate2, tempBoard, h, &tempMdlme , &tempMdlhim , tempDice);
				}
			}
			revDubleChanges(rev1 , &ate1, tempBoard, i, &tempMdlme , &tempMdlhim , tempDice);
		}
	}
}

Node::Node(int* array , int amdlMe , int amdlHim ,pair<int,int> currentDice)
{	
	pair<int,int> p(0,0);
	m_move1 = p;
	m_move2 = p;
	m_move3 = p;
	m_move4 = p;
	mdlMe = amdlMe;
	mdlHim = amdlHim;
	m_dice = currentDice;
	m_board[25];
	memcpy(m_board,array, sizeof(int) * 25);
	heurristic_val = 0;
	/*whatMoveAmI1 = " ";
	whatMoveAmI2 = " ";
	whatMoveAmI3 = " ";
	whatMoveAmI4 = " ";*/
	whiteLevel = false;

	idDice = 0;
}

Node::~Node(void)
{
}

void Player::getRemainingDice(pair<int,int>* my_bag, pair<int,int>* his_bag)
{
	int arg1;
	int arg2;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			a_remainingDice[i][j] = 0;
			bagForLevel[i][j] = 0;
	        a_remainingDiceOpponent[i][j] = 0;
		}
	}

	for (int i = 0; i < DICENUM; i++)
	{
		if ((my_bag[i].first!=-1))
		{
			if (my_bag[i].first >my_bag[i].second) 
			{
				arg1 = my_bag[i].second;
				arg2 = my_bag[i].first;
			}
			else
			{
				arg2 = my_bag[i].second;
				arg1 = my_bag[i].first;
			}
			a_remainingDice[arg1][arg2]++;
			bagForLevel[arg1][arg2]++;
		}		
	}
	
		for (int i = 0; i < DICENUM; i++)
	{
		if ((his_bag[i].first!=-1))
		{
			if (his_bag[i].first >his_bag[i].second) 
			{
				arg1 = his_bag[i].second;
				arg2 = his_bag[i].first;
			}
			else
			{
				arg2 = his_bag[i].second;
				arg1 = his_bag[i].first;
			}
			a_remainingDiceOpponent[arg1][arg2]++;

		}		
	}
	
return;
}

void Player::swapBagForDiffrentLevel(bool toCurrent)
{
	if(toCurrent)
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				bagForLevel[i][j] = a_remainingDice[i][j]; 
	else
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				bagForLevel[i][j] = a_remainingDiceOpponent[i][j]; 
}

void Player::destroyLevel(Node* father)
{
	
	for(Node* temp : father->children)
	{
		if (temp->children.size() > 0) 
			destroyLevel(temp);
		delete(temp);
		temp = NULL;
		Player::memoryTrace--;
	}
}

int Player::chekcDoubleEntry(int* tempBoard, int* tempMdlme, int* tempMdlhim, int moveStart, int tempDice, bool* pin , bool* ate)
{
	bool canRemove = true; //fix fetData()
	if (*tempMdlme > 0 ) canRemove = false;
	else
	{
		for (int i = 1; i < 19; i++)
		{
			if (tempBoard[i] > 0 )  	{ canRemove = false;	break; }
		}
	}	

	bool a1 = ((*tempMdlme > 0) && (tempBoard[tempDice] >= -1)); // mdl pieces
	stucked = ((*tempMdlme > 0) && (tempBoard[tempDice] < -1));
	bool a2 = (tempBoard[moveStart] > 0) && (moveStart + tempDice <= 24) && (tempBoard[moveStart + tempDice] > -2); // regular move
	bool a3 = (moveStart + tempDice > 24) && (canRemove) && (tempBoard[moveStart] > 0) && (!(man->val->canRemove2(moveStart , tempBoard ,tempDice ))) ; // can remove the piece



	if(stucked) return 5;
	if(a1)
	{
		if(tempBoard[tempDice] == -1)
		{
			(*tempMdlhim)++;
			tempBoard[tempDice] = 0;
			*ate = true;
		}
		tempBoard[tempDice]++;
		(*tempMdlme)--;
		*pin = true;
		return 1;
	}
	else if(a2)
	{
		if(tempBoard[moveStart + tempDice] == -1)
		{
			(*tempMdlhim)++;
			tempBoard[moveStart + tempDice] = 0;
			*ate = true;
		}
		tempBoard[moveStart]--;
		tempBoard[moveStart + tempDice]++;//#bug half move
		return 2;
	}
	else if(a3)
	{
		tempBoard[moveStart]--;
		//outCurrent ++
		return 3;
	}
	return 0;
}

void Player::revDubleChanges(int rev,bool* ate, int* tempBoard,int moveStart,int* tempMdlme,int *tempMdlhim , int tempDice )
{
		
	if(rev == 1)
	{
		tempBoard[tempDice]--;
		(*tempMdlme)++;
		if (*ate)
		{
			(*tempMdlhim)--;
			tempBoard[tempDice] = -1;
			*ate = false;
		}
	}
	else if(rev == 2)
	{
		tempBoard[moveStart]++;
		tempBoard[moveStart + tempDice]--;	
		if (*ate)
		{
			(*tempMdlhim)--;
			tempBoard[moveStart + tempDice] = -1;
			*ate = false;
		}
	}
	else if(rev == 3)
	{
		tempBoard[moveStart]++;
		//outCurrent --
	}
	return;
}

pair<int,int> Player::sacrificeADice()
{
	destroyLevel(root);
	pair<int,int> p (-5,-5);
	for(int i=0; i<DICENUM ; i++)
	{
		if((a_bag[i].first != -1) && (a_bag[i].first != a_bag[i].second ))
			return a_bag[i];
	}
	for(int i=0; i<DICENUM ; i++)
	{
		if(a_bag[i].first != -1)
		    return a_bag[i];
	}
	return p;

}


Node * Player::halfMoveChooseWhatToDoWhiteStart() 
{
	delete(root);
	root= NULL;
	allTreeNodes = 1;
	getRemainingDice(a_bag,man->pBlack->a_bag);
	root = new Node(man->b->a_board ,man->b->a_mdl[1],man->b->a_mdl[0], pair<int,int>(-1,-1));
	root->whiteLevel = false;
	buildhalfLevel();
	return root->children.size() > 0 ? root->children.at(mudul%(root->children.size())) : NULL;
}

Node * Player::halfMoveChooseWhatToDoBlackStart() 
{
	delete(root);
	root= NULL;
	allTreeNodes = 1;
	getRemainingDice(a_bag,man->pWhite->a_bag);
	root = new Node(man->b->a_rboard ,man->b->a_mdl[0],man->b->a_mdl[1], pair<int,int>(-1,-1));
	root->whiteLevel = true;
	buildhalfLevel();
	return root->children.size() > 0 ? root->children.at(mudul%(root->children.size())) : NULL;
}

void Player::buildhalfLevel()
{
	memcpy(updateNode->m_board ,root->m_board,sizeof(int) *25);
	updateNode->mdlMe = root->mdlHim;
	
	for (int i = 1; i < 7; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (bagForLevel[i][j] > 0) 
			{
				i==j ? man->turns = 4 : man->turns = 2;
				man->_dice = pair<int,int>(i,j);	
				bool x = false;
				bool isDubel = (i == j);
				
				if(isDubel)
				{
					x = !(man->val->dubelCannotMove());//player data !
					if (x)
					{
						man->turns = 4;
						searchHalfDoubleMove(i);
					}
					
				}
				else
				{
					x = !(man->val->regCannotMove());
					if (x)
					{
						man->turns = 2;
						searchHlafRegMove(pair<int, int>(i, j));
					}
				}
				man->val->getDataUnecessary = false;
			} 
		}
	}
	man->val->getDataUnecessary = true;
}

void Player::searchHlafRegMove( pair<int,int>& tempDice)
{
	int idDice=0;
	int tempBoard[25];
	bool pin = false;
	bool ate = false;
	int tempMdlme  = root->mdlHim;
	int tempMdlhim = root->mdlMe;

	memcpy(tempBoard,root->m_board, sizeof(int) * 25);
	for (int k = 0; k < 2; k++)
	{
		if (k==1)
		{
			pin = false;
			int swap = tempDice.second; 
			tempDice.second =tempDice.first;
			tempDice.first = swap;
		}
		for (int j = 1; j < 25 && pin==false ; j++)
		{	
			if ( int rev = chekcDoubleEntry(tempBoard,  &tempMdlme, &tempMdlhim,  j,  tempDice.first, &pin , &ate) )
			{
				if(stucked) break;
				pair<int,int> tempKey;
				int temp;
				pin ? temp = tempDice.first : temp = j;
				tempKey.first  = temp;
				tempKey.second = temp + tempDice.first ;	
				Node* myNode = new Node(tempBoard,tempMdlme,tempMdlhim,tempDice);
				Player::memoryTrace++;
				myNode->whiteLevel = !(root->whiteLevel);
				allTreeNodes++;					
				myNode->m_move1= tempKey;
				if ( myNode->m_move1.second > 24) { myNode->m_move1.second = 25;}
				else if (pin)					  { myNode->m_move1.second = myNode->m_move1.first;}
				myNode->m_move2 = pair<int,int>(-3,-3);
				myNode->idDice = ++idDice;
				root->children.push_back(myNode); 						
				revDubleChanges(rev , &ate , tempBoard , j, &tempMdlme , &tempMdlhim , tempDice.first);
			}
		} 
	}
}

void Player::searchHalfDoubleMove(int& tempDice )
{
	bool pin1,pin2,pin3;
	bool ate1,ate2,ate3;
	pin1=pin2=pin3=false;
	ate1=ate2=ate3=false;
	int idDice=0;
	int tempBoard[25];
	int tempMdlme  = root->mdlHim;
	int tempMdlhim = root->mdlMe;

	memcpy(tempBoard,root->m_board, sizeof(int) * 25);
	for (int h = 1; h < 25 && pin1==false; h++)
	{
		if(int rev1 = chekcDoubleEntry(tempBoard,&tempMdlme , &tempMdlhim , h ,tempDice , &pin1 , &ate1) )
		{
			bool made1 = true;
			for (int k = h; k < 25 && pin2==false; k++)
			{
				if(int rev2 = chekcDoubleEntry(tempBoard,&tempMdlme , &tempMdlhim , k ,tempDice , &pin2 , &ate2) )
				{
					bool made2 = true;
					for (int j = k; j < 25 && pin3==false; j++)
					{	
						if(int rev3 = chekcDoubleEntry(tempBoard,&tempMdlme , &tempMdlhim , j ,tempDice , &pin3 , &ate3) )
						{
							Node* myNode = new Node(tempBoard,tempMdlme,tempMdlhim,pair<int,int>(tempDice,tempDice));
							Player::memoryTrace++;
							int temp;
							allTreeNodes++;
							myNode->whiteLevel = !(root->whiteLevel);
							pin1 ? temp = tempDice : temp =h;
							myNode->m_move1.first  = temp; 
							myNode->m_move1.second = temp + tempDice; 
							if ( myNode->m_move1.second > 24) { myNode->m_move1.second = 25;}
							else if (pin1)					  { myNode->m_move1.second = myNode->m_move1.first;}
							pin2? temp = tempDice : temp = k;
							myNode->m_move2.first  = temp; 
							myNode->m_move2.second = temp + tempDice; 
							if ( myNode->m_move2.second > 24) { myNode->m_move2.second = 25;}
							else if (pin2)					  { myNode->m_move2.second = myNode->m_move2.first;}

							pin3? temp = tempDice : temp = j;
							myNode->m_move3.first = temp;
							myNode->m_move3.second= temp +  tempDice;
							if ( myNode->m_move3.second > 24) {  myNode->m_move3.second = 25;}
							else if (pin3)					  {  myNode->m_move3.second = myNode->m_move3.first;}
							myNode->m_move4 = pair<int,int> (-3,-3); 		
							myNode->idDice = ++idDice;
							root->children.push_back(myNode);
							revDubleChanges(rev3 , &ate3 , tempBoard, j, &tempMdlme , &tempMdlhim , tempDice);
							made1 = made2 = false;
						}
					} 
					if(made2)
					{
						Node* myNode = new Node(tempBoard,tempMdlme,tempMdlhim,pair<int,int>(tempDice,tempDice));
						Player::memoryTrace++;
						int temp;
						allTreeNodes++;
						myNode->whiteLevel = !(root->whiteLevel);
						pin1 ? temp = tempDice : temp =h;
						myNode->m_move1.first  = temp; 
						myNode->m_move1.second = temp + tempDice; 
						if ( myNode->m_move1.second > 24) {  myNode->m_move1.second = 25;}
						else if (pin1)					  {  myNode->m_move1.second = myNode->m_move1.first;}
						pin2? temp = tempDice : temp = k;
						myNode->m_move2.first  = temp; 
						myNode->m_move2.second = temp + tempDice; 
						if ( myNode->m_move2.second > 24) {  myNode->m_move2.second = 25;}
						else if (pin2)					  {  myNode->m_move2.second = myNode->m_move2.first;}
						myNode->m_move3 = pair<int,int> (-3,-3);
						myNode->m_move4 = pair<int,int> (-3,-3); 									
						myNode->idDice = ++idDice;
						root->children.push_back(myNode);
						made1 = false;
					}
					revDubleChanges(rev2 , &ate2, tempBoard, k, &tempMdlme , &tempMdlhim , tempDice);
				}	
			} 
			if (made1)
			{
				Node* myNode = new Node(tempBoard,tempMdlme,tempMdlhim,pair<int,int>(tempDice,tempDice));
				Player::memoryTrace++;
				int temp;
				allTreeNodes++;
				myNode->whiteLevel = !(root->whiteLevel);
				pin1 ? temp = tempDice : temp =h;
				myNode->m_move1.first  = temp; 
				myNode->m_move1.second = temp + tempDice;
				if ( myNode->m_move1.second > 24) {  myNode->m_move1.second = 25;}
				else if (pin1)					  {  myNode->m_move1.second = myNode->m_move1.first;}
				myNode->m_move2 = pair<int,int> (-3,-3);
				myNode->m_move3 = pair<int,int> (-3,-3);
				myNode->m_move4 = pair<int,int> (-3,-3); 									
				myNode->idDice = ++idDice;
				root->children.push_back(myNode);
				made1 = false;
			}
			revDubleChanges(rev1 , &ate1, tempBoard, h, &tempMdlme , &tempMdlhim , tempDice);
		}
	}
}
