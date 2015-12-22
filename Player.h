#ifndef _PLAYER_H_
#define _PLAYER_H_
#pragma once


#include <stdlib.h>    
#include <time.h> 

#include "Manager.h"

using namespace std;

class Node;
class Manager;

void switch_board(int* brd);

class Player
{
public:
	Player(bool);
	void initPointers();
	pair<int,int> chooseMove();
	pair<int,int> chooseDice();
	~Player(void);

	int n_DiceInBag;
	string s_color;
	pair<int,int> a_bag[DICENUM];
	bool _isHuman;
	int lastTry;
	int round;
	int turn;
	int a_remainingDice[7][7];
	int a_remainingDiceOpponent[7][7];
	Manager* man;
	int boardForLevel[25];
	int bagForLevel[7][7];
	bool isWhitePlayer;
	int a_moveTransfer[25]; //so when black play 24->23 it will be seen as 1->2

	int allTreeNodes;
	void getRemainingDice(pair<int,int>*,pair<int,int>*);
	void swapBagForDiffrentLevel(bool);

	static int memoryTrace;

	Node* player_root;
	bool doSwapMove;
	pair<int,int> chosenMove;
	pair<int,int> chosenDice;
	Node* chooseWhatToDo();

	void build_tree(Node* root, int depth, bool my_move, bool minmax);

	void buildOneLevel(Node *);
	void searchRegMove(Node *,pair<int,int>& );
	void searchDoubleMove(Node *,int& );
	Node* theChoise;
	Node* updateNode;
	void destroyLevel(Node*);
	int chekcDoubleEntry(int* , int* , int* , int , int, bool* , bool*);
	void revDubleChanges(int ,bool* , int* ,int ,int* ,int * , int  );
	bool stucked;

	pair<int,int> sacrificeADice();

	void buildhalfLevel(Node* root);
	void searchHlafRegMove( pair<int,int>& tempDice, Node* root);
	void searchHalfDoubleMove(int& tempDice, Node* root );


	int mudul;
};

class Node
{
public:
	Node(int *,int,int,pair<int,int>);
	~Node(void);

	pair<int,int> m_dice;
	double heurristic_val;
	pair<int,int> m_move1;
	pair<int,int> m_move2;
	pair<int,int> m_move3;
	pair<int,int> m_move4;

	int mdlMe;
	int mdlHim;
	int m_board[25];
	int idDice;
	vector<Node*> children;
	bool whiteLevel;
};



#endif