#include "Player.h"
#include "Manager.h"

int Player::memoryTrace = 0;

Player::Player(bool player_color, int dice_version, int player_type)
{
	round = 0;
	turn = -1;
	isWhitePlayer = player_color;
	_isHuman = player_type == 1;
	player_color ? s_color="WHITE" : s_color = "BLACK"; 
	dice_version == 1 ? n_DiceInBag = DICENUM_VERSION1 : n_DiceInBag = DICENUM_VERSION2;
	man = NULL;
	init_dice_matrix(dice_version);

	fill_n(a_moveTransfer, 25, 0);
	int num = 24;
	for (int i = 1; i < 25; i++)
	{
		a_moveTransfer[i] = num--; 
	}
	
	doSwapMove = false;
	stucked = false;
	theChoise = NULL;
	player_root = new Node(boardForLevel , 0 , 0, my_pair(-1,-1));
	updateNode = new Node(boardForLevel , 0 , 0, my_pair(-1,-1));
	lastTry = -1;
	allTreeNodes = -1;

}

void Player::init_dice_matrix(int game_version)
{
	if (game_version == 1)
	{
		for (int i = 0; i < 7; i++){
			for (int j = 0; j < 7; j++){
				if (i == j && i != 0 && j != 0)
					m_bag[i][j] = 1;
				else if (i < j && i != 0 && j != 0)
					m_bag[i][j] = 2;
				else
					m_bag[i][j] = -1;
			}
		}

		srand((unsigned int)time(NULL));
		for (int i = 0; i < 10; i++)
		{
			int dice1 = rand() % 6 + 1;
			int dice2 = rand() % 6 + 1;
			if (dice1 > dice2)
			{
				int tmp = dice1; dice1 = dice2; dice2 = tmp;
			}
			m_bag[dice1][dice2]++;
		}
	}
	else  // game version #2
	{
		for (int i = 0; i < 7; i++){
			for (int j = 0; j < 7; j++){
				if (i == j && i != 0 && j != 0)
					m_bag[i][j] = 2;
				else if (i < j && i != 0 && j != 0)
					m_bag[i][j] = 4;
				else
					m_bag[i][j] = -1;
			}
		}
	}
}

Player::~Player(void)
{
}

my_pair Player::chooseDice()
{
	my_pair p(-5,-5);
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
		man->val->doneCumputingMoves = false; 
		theChoise = chooseWhatToDo();
		
		if(theChoise)
		{
			p = theChoise->m_dice;
		}
		//else // the choise == null - no children to root.
		//{			
		//		if (! PLAYINGAGAINSMAYANDROTEM)  p = sacrificeADice(); 
		//}
		


	}
	man->val->doneCumputingMoves = true;
	return p; //p == -5,-5  default
}

my_pair Player::chooseMove()
{
	string input;
	my_pair p(0,0);
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

void switch_board(int* brd)
{
	for (int i = 1, j = 24; i < j; i++,j--)
	{
		int tmp = brd[i] * (-1);
		brd[i] = brd[j] * (-1);
		brd[j] = tmp;
	}
}

Node* Player::chooseWhatToDo()
{
	Node* best_child = NULL;
	delete(player_root);
	player_root = NULL;
	bool is_white = man->bIsWhiteTurn;
	allTreeNodes = 1;

	getRemainingDice(m_bag, is_white ? man->pWhite->m_bag : man->pBlack->m_bag);

	for (int i = 0; i < 25; i++) { boardForLevel[i] = is_white ? man->b->a_rboard[i]: man->b->a_board[i]; }

	// a_mdl[0] is white middle
	player_root = new Node(boardForLevel, man->b->a_mdl[is_white], man->b->a_mdl[!is_white], my_pair(-1, -1));
	player_root->whiteLevel = !is_white;

	if (man->alpha_beta)
		alpha_beta(player_root, man->maxDepth, true, INIT_HEURISTIC_VAL_MIN, INIT_HEURISTIC_VAL_MAX);
	else
		mini_max(player_root, man->maxDepth, true);

	for (Node* child : player_root->children)
		if (child->heurristic_val >= player_root->heurristic_val) {
			player_root->heurristic_val = child->heurristic_val;
			best_child = child;
		}
	
	return best_child;
}	


void Player::alpha_beta(Node* root, int depth, bool maximizing_move, double alpha, double beta)
{
	root->alpha = alpha; root->beta = beta;

	buildOneLevel(root, true, depth == 1, maximizing_move);

	if (root->children.empty())
		buildhalfLevel(root);

	if (root->children.empty())
		return;

	swapBagForDiffrentLevel(maximizing_move);
	for (Node* child : root->children)
	{
		if (depth > 1)
		{
			alpha_beta(child, depth - 1, !maximizing_move, root->alpha, root->beta);
		}

		if (root->heurristic_val == INIT_HEURISTIC_VAL_MIN)
			root->heurristic_val = child->heurristic_val;

		if (maximizing_move) {
			if (child->heurristic_val > root->heurristic_val)
				root->heurristic_val = child->heurristic_val;
			if (child->heurristic_val > root->alpha)
				root->alpha = child->heurristic_val;
		}
		else if (!maximizing_move) {
			if (child->heurristic_val < root->heurristic_val)
				root->heurristic_val = child->heurristic_val;
			if (child->heurristic_val < root->beta)
				root->beta = child->heurristic_val;
		}

		if (root->alpha >= root->beta) {
			if (root->children.size())
				root->heurristic_val = root->children[root->children.size() - 1]->heurristic_val;
			if (root != player_root)
				destroyLevel(root);
			return;	/*prune rest of the tree*/
		}
	}
	if (root != player_root)	/*don't delete first level*/
		destroyLevel(root);
}

void Player::mini_max(Node* root, int depth, bool my_move)
{
	buildOneLevel(root, false, false, false);

	if (root->children.empty())
		buildhalfLevel(root);

	if (root->children.empty())
		return;

	swapBagForDiffrentLevel(my_move);
	for (Node* child : root->children)
	{
		if (depth > 1)
		{
			//a_remainingDice[child->m_dice.first][child->m_dice.second]--;
			mini_max(child, depth - 1, !my_move);
			//a_remainingDice[child->m_dice.first][child->m_dice.second]++;
		}
		else  // tree leafs - use heuristics and pass values up the tree
		{
			child->heurristic_val = man->strategy_ptr->evaluate_node(child, my_move);
			if (root->heurristic_val == INIT_HEURISTIC_VAL_MIN)
				root->heurristic_val = child->heurristic_val;
			else 
			{
				if ((my_move && child->heurristic_val > root->heurristic_val) ||
					(!my_move && child->heurristic_val < root->heurristic_val))
					root->heurristic_val = child->heurristic_val;
			}
		}
		if (root != player_root){	/*don't delete first level*/
			delete(child);
			Player::memoryTrace--;
		}
	}
}

void Player::initPointers()
{
	man = Manager::instance();
	return;
}

void Player::buildOneLevel(Node *root, bool alpha_beta, bool is_last, bool root_max_move) 
{
	for (int i = 0; i < 25; i++)
	{
		boardForLevel[a_moveTransfer[i]] = root->m_board[i]* (-1); 
		updateNode->m_board[a_moveTransfer[i]] = root->m_board[i]* (-1); 
	}
	updateNode->mdlMe = root->mdlHim;
	
	for (int i = 6; i > 0; i--)
	{
		for (int j = 6; j > 0; j--)
		{
			if (bagForLevel[i][j] > 0) 
			{
				man->_dice = my_pair(i,j);	
				int start_child = root->children.empty()?0:root->children.size();
				if(i == j)	// Dubel
				{
					man->turns = 4;
					if (!(man->val->dubelCannotMove()))
					{	
						man->turns = 4;
						searchDoubleMove(root, i);
					}
				}
				else		// regular move
				{
					man->turns = 2;
					if (!(man->val->regCannotMove()))
					{	
						man->turns = 2;
						searchRegMove(root, my_pair(i, j));
					}
				}
				int end_child = root->children.size();
				if (alpha_beta && is_last)
				{
					for (int i = start_child; i < end_child; i++)
					{
						root->children[i]->heurristic_val = man->strategy_ptr->evaluate_node(root->children[i], root_max_move);
						if (root_max_move) {
							if (root->children[i]->heurristic_val > root->alpha)
								root->alpha = root->children[i]->heurristic_val;
						}
						else if (!root_max_move){
							if (root->children[i]->heurristic_val < root->beta)
								root->beta = root->children[i]->heurristic_val;
						}
						if (root->alpha >= root->beta) {
							man->val->getDataUnecessary = true;
							return;
						}
					}
				}
				man->val->getDataUnecessary = false;
			} 
		}
	}
	man->val->getDataUnecessary = true;
}

void Player::searchRegMove(Node *levelFather, my_pair& tempDice)
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
						my_pair move1;
						my_pair move2;
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
									Node* myNode = new Node(tempBoard,tempMdlme,tempMdlhim,my_pair(tempDice,tempDice));
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

Node::Node(int* array , int amdlMe , int amdlHim ,my_pair currentDice)
{	
	my_pair p(0,0);
	m_move1 = p;
	m_move2 = p;
	m_move3 = p;
	m_move4 = p;
	mdlMe = amdlMe;
	mdlHim = amdlHim;
	m_dice = currentDice;
	m_board[25];
	memcpy(m_board,array, sizeof(int) * 25);
	heurristic_val = INIT_HEURISTIC_VAL_MIN;
	alpha = INIT_HEURISTIC_VAL_MIN;
	beta = INIT_HEURISTIC_VAL_MAX;
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
void Player::getRemainingDice(dice_matrix my_bag, dice_matrix his_bag)
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			a_remainingDice[i][j] = my_bag[i][j];
			bagForLevel[i][j] = my_bag[i][j];
			a_remainingDiceOpponent[i][j] = his_bag[i][j];
		}
	}
}

void Player::swapBagForDiffrentLevel(bool toCurrent)
{
		if (toCurrent)
			for (int i = 0; i < 7; i++)
				for (int j = 0; j < 7; j++)
					bagForLevel[i][j] = a_remainingDice[i][j];
		else
			if (man->dice_version == 1) /// no need to copy  bag is shared
				for (int i = 0; i < 7; i++)
					for (int j = 0; j < 7; j++)
						bagForLevel[i][j] = a_remainingDiceOpponent[i][j];
}

void Player::destroyLevel(Node* father)
{
	for(Node* temp : father->children)
	{
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

//my_pair Player::sacrificeADice()
//{
//	destroyLevel(player_root);
//	my_pair p (-5,-5);
//	for(int i=0; i<DICENUM_VERSION1 ; i++)
//	{
//		if((a_bag[i].first != -1) && (a_bag[i].first != a_bag[i].second ))
//			return a_bag[i];
//	}
//	for(int i=0; i<DICENUM_VERSION1 ; i++)
//	{
//		if(a_bag[i].first != -1)
//		    return a_bag[i];
//	}
//	return p;
//
//}


void Player::buildhalfLevel(Node* root)
{
	//for (int i = 0; i < 25; i++)
	//{
	//	boardForLevel[a_moveTransfer[i]] = root->m_board[i] * (-1);
	//	updateNode->m_board[a_moveTransfer[i]] = root->m_board[i] * (-1);
	//}
	//updateNode->mdlMe = root->mdlHim;
	
	for (int i = 1; i < 7; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (bagForLevel[i][j] > 0) 
			{
				i==j ? man->turns = 4 : man->turns = 2;
				man->_dice = my_pair(i,j);	
				bool x = false;
				bool isDubel = (i == j);
				
				if(isDubel)
				{
					x = !(man->val->dubelCannotMove());//player data !
					if (x)
					{
						man->turns = 4;
						searchHalfDoubleMove(i, root);
					}
					
				}
				else
				{
					x = !(man->val->regCannotMove());
					if (x)
					{
						man->turns = 2;
						searchHlafRegMove(my_pair(i, j) , root);
					}
				}
				man->val->getDataUnecessary = false;
			} 
		}
	}
	man->val->getDataUnecessary = true;
}
void Player::searchHlafRegMove( my_pair& tempDice, Node* root)
{
	int idDice=0;
	int tempBoard[25];
	bool pin = false;
	bool ate = false;
	int tempMdlme  = root->mdlHim;
	int tempMdlhim = root->mdlMe;

	memcpy(tempBoard,boardForLevel, sizeof(int) * 25);
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
				my_pair tempKey;
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
				myNode->m_move2 = my_pair(-3,-3);
				myNode->idDice = ++idDice;
				root->children.push_back(myNode); 						
				revDubleChanges(rev , &ate , tempBoard , j, &tempMdlme , &tempMdlhim , tempDice.first);
			}
		} 
	}
}

void Player::searchHalfDoubleMove(int& tempDice, Node* root )
{
	bool pin1,pin2,pin3;
	bool ate1,ate2,ate3;
	pin1=pin2=pin3=false;
	ate1=ate2=ate3=false;
	int idDice=0;
	int tempBoard[25];
	int tempMdlme  = root->mdlHim;
	int tempMdlhim = root->mdlMe;

	memcpy(tempBoard,boardForLevel, sizeof(int) * 25);
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
							Node* myNode = new Node(tempBoard,tempMdlme,tempMdlhim,my_pair(tempDice,tempDice));
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
							myNode->m_move4 = my_pair (-3,-3); 		
							myNode->idDice = ++idDice;
							root->children.push_back(myNode);
							revDubleChanges(rev3 , &ate3 , tempBoard, j, &tempMdlme , &tempMdlhim , tempDice);
							made1 = made2 = false;
						}
					} 
					if(made2)
					{
						Node* myNode = new Node(tempBoard,tempMdlme,tempMdlhim,my_pair(tempDice,tempDice));
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
						myNode->m_move3 = my_pair (-3,-3);
						myNode->m_move4 = my_pair (-3,-3); 									
						myNode->idDice = ++idDice;
						root->children.push_back(myNode);
						made1 = false;
					}
					revDubleChanges(rev2 , &ate2, tempBoard, k, &tempMdlme , &tempMdlhim , tempDice);
				}	
			} 
			if (made1)
			{
				Node* myNode = new Node(tempBoard,tempMdlme,tempMdlhim,my_pair(tempDice,tempDice));
				Player::memoryTrace++;
				int temp;
				allTreeNodes++;
				myNode->whiteLevel = !(root->whiteLevel);
				pin1 ? temp = tempDice : temp =h;
				myNode->m_move1.first  = temp; 
				myNode->m_move1.second = temp + tempDice;
				if ( myNode->m_move1.second > 24) {  myNode->m_move1.second = 25;}
				else if (pin1)					  {  myNode->m_move1.second = myNode->m_move1.first;}
				myNode->m_move2 = my_pair (-3,-3);
				myNode->m_move3 = my_pair (-3,-3);
				myNode->m_move4 = my_pair (-3,-3); 									
				myNode->idDice = ++idDice;
				root->children.push_back(myNode);
				made1 = false;
			}
			revDubleChanges(rev1 , &ate1, tempBoard, h, &tempMdlme , &tempMdlhim , tempDice);
		}
	}
}
