#ifndef _MANAGER_H_
#define _MANAGER_H_
#pragma once

#define DICENUM_VERSION1 46
#define DICENUM_VERSION2 72
#define PLAYINGAGAINSMAYANDROTEM 0
#define HOWMANYGAMESINAROW 0
#define DEBUG val->msg4="move from "+std::to_string(_move.first)+" to "+std::to_string(_move.second);
#define PLAYER1 0
#define PLAYER2 0
#define TREELEVELS 2
#define INF 10000
#define INIT_HEURISTIC_VAL numeric_limits<double>::lowest()



#include <iostream>
#include <string>
#include <vector>

typedef std::pair<int, int> my_pair;
typedef int dice_matrix[7][7];
using namespace std;

#include "Board.h"
#include "Player.h"
#include "Validator.h"
#include "strategy.h"
#include <assert.h>
#include <time.h> 
#include <stdlib.h>    

class Board;
class Validator;
class Player;
class strategy;

//singleton
class Manager
{
public:
	static Manager* instance();
	static void ResetInstance();

	void initPointersOfMembers();
	void runGame();
	void showHelp();
	void swapDice();
	~Manager(void);
	void gameOver(string msg1 = " ",string msg2= " " ,string msg3= " ",string msg4= " ");
	void playDice();
	//void playRegularDice();
	
	string sDice;

	bool bIsWhiteTurn;
	bool bSwapDice;
	bool bCurrentDiceDubel;
	bool isDiceLegal;
	pair<int , int> _move;
	pair<int , int> _dice;
	int turns;

	int* currentDice;

	Board *b; 
	Player *pWhite; 
	Player *pBlack;
	Player *currentPlayer;
	Validator *val;
	int maxDepth;

	int games;
	int dice_version;
	strategy *strategy_ptr;
	bool alpha_beta;
	int player1;
	int player2;
private:	
	static Manager* m_instance;
	Manager(void);

};



#endif