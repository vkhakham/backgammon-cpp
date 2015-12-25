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
	Player(bool player_color, int dice_version, int player_type);

	void init_dice_matrix(int game_version);

	void initPointers();
	my_pair chooseMove();
	my_pair chooseDice();
	~Player(void);

	int n_DiceInBag;
	string s_color;
	//my_pair a_bag[DICENUM];
	dice_matrix m_bag;
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
	void getRemainingDice(dice_matrix my_bag, dice_matrix his_bag);
	void swapBagForDiffrentLevel(bool);

	static int memoryTrace;

	Node* player_root;
	bool doSwapMove;
	my_pair chosenMove;
	my_pair chosenDice;
	Node* chooseWhatToDo();

	void mini_max(Node* root, int depth, bool my_move);
	void alpha_beta(Node* root, int depth, bool maximizing_move, double alpha, double beta);

	void buildOneLevel(Node *, bool alpha_beta, bool is_last, bool max_move);
	void searchRegMove(Node *,my_pair& );
	void searchDoubleMove(Node *,int& );
	Node* theChoise;
	Node* updateNode;
	void destroyLevel(Node*);
	int chekcDoubleEntry(int* , int* , int* , int , int, bool* , bool*);
	void revDubleChanges(int ,bool* , int* ,int ,int* ,int * , int  );
	bool stucked;

	//my_pair sacrificeADice();

	void buildhalfLevel(Node* root);
	void searchHlafRegMove( my_pair& tempDice, Node* root);
	void searchHalfDoubleMove(int& tempDice, Node* root );

	int mudul;
};

class Node
{
public:
	Node(int *,int,int,my_pair);
	~Node(void);

	my_pair m_dice;
	double heurristic_val;
	double alpha;
	double beta;
	my_pair m_move1;
	my_pair m_move2;
	my_pair m_move3;
	my_pair m_move4;

	int mdlMe;
	int mdlHim;
	int m_board[25];
	int idDice;
	vector<Node*> children;
	bool whiteLevel;
};



#endif