#ifndef _PLAYER_H_
#define _PLAYER_H_
#pragma once


#include <stdlib.h>    
#include <time.h> 

#include "Manager.h"

using namespace std;

class Node;
class Manager;

class Player
{
public:
	Player(bool);
	void initPointers();
	pair<int,int> chooseMove();
	pair<int,int> chooseDice();
	pair<int,int> chooseFirstDiceFromBag();
	pair<int,int> moveLongestPiece();
	~Player(void);

	int n_DiceInBag;
	string s_color;
	pair<int,int> a_bag[46];
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

	Node* root;
	bool doSwap;
	bool doSwapMove;
	pair<int,int> chosenMove;
	pair<int,int> chosenDice;
	Node* chooseWhatToDoWhiteStart();
	Node* chooseWhatToDoBlackStart();
	void buildOneLevel(Node *);
	void searchRegMove(Node *,pair<int,int>& );
	void searchDoubleMove(Node *,int& );
	Node* theChoise;
	Node* updateNode;
	void destroyLevel(Node*);
	int chekcDoubleEntry(int* , int* , int* , int , int, bool* , bool*);
	void revDubleChanges(int ,bool* , int* ,int ,int* ,int * , int  );
	bool stucked;

	Node *halfMoveChooseWhatToDoWhiteStart();
	Node *halfMoveChooseWhatToDoBlackStart();
	pair<int,int> sacrificeADice();

	void buildhalfLevel();
	void searchHlafRegMove( pair<int,int>& tempDice);
	void searchHalfDoubleMove(int& tempDice );


	int mudul;
};

class Node
{
public:
	Node(int *,int,int,pair<int,int>);
	~Node(void);

	pair<int,int> m_dice;
	pair<int,int> m_move1;
	pair<int,int> m_move2;
	pair<int,int> m_move3;
	pair<int,int> m_move4;
	/*string whatMoveAmI1; //regular , going down , removing piece
	string whatMoveAmI2;
	string whatMoveAmI3;
	string whatMoveAmI4;*/
	int mdlMe;
	int mdlHim;
	int m_board[25];
	int idDice;
	vector<Node*> children;
	bool whiteLevel;
};



#endif