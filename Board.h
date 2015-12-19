#ifndef _BOARD_H_
#define _BOARD_H_
#pragma once

#include <vector>
#include "Manager.h"

using namespace std;
class Player;
class Manager;

//singleton
class Board
{
public:	
	static Board* instance();
	static void ResetInstance();

	~Board();
	void initPointers();
	void printBoard(string msg1 = " " ,string msg2 = " ",string msg3 = " " ,string msg4 = " " );
	void initBoard();
	void emitBoard();
	bool turnPrintAllocationPrints;

	int a_board[25];   //the board
	int a_rboard[25];  //the reverse board. we only sync it when black is playing
	int a_mdl[2];	   //the "ON MIDDLE" pieces	   
	int a_out[2];	  //the pieces the player got out
	int n_round;
	int a_moveTransfer[25]; //so when black play 24->23 it will be seen as 1->2
	Manager* man;

private:
	static Board* b_instance;
	Board();
};



#endif
