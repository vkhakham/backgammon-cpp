#ifndef _VALIDATOR_H_
#define _VALIDATOR_H_
#pragma once


#include "Manager.h"

using namespace std;
class Manager;


//singleton
class Validator
{
public:
	static Validator* instance();
	static void ResetInstance();
	void initPointers();
	void flushMsg();
	~Validator(void);

	bool checkDice ();
	bool diceOutOfRange();

	bool checkMove ();
	bool moveOutOfRange();
	bool piecesAreOnMdl();
	bool canRemove(int );
	
	void getPlayerData();
	
	bool dubelCannotMove();
	bool dubelPlayerCannotComeBackFromMiddle();
	bool dubelNoRegularMove();
	
	bool regCannotMove();
	bool regPlayerCannotComeBackFromMiddle();
	bool regNoRegularMove();
	bool canRemove2(int ,int* , int);
	bool doneCumputingMoves;
	bool getDataUnecessary;

	string msg1;
	string msg2;
	string msg3;
	string msg4;
	Manager* man;
	int* aBoard;
	int mdlPieces;
	bool canRemovePieces;
	int *currentDice;
	my_pair move;

private:
	Validator(void);
	static Validator* v_instance;
};



#endif