#ifndef _MANAGER_H_
#define _MANAGER_H_
#pragma once

#define PLAYINGAGAINSMATANDROTEM 0
#define HOWMANYGAMESINAROW 2
#define MUDUL 46


#include <iostream>
#include <string>
#include <vector>


#include "Board.h"
#include "Player.h"
#include "Validator.h"


using namespace std;

class Board;
class Validator;
class Player;

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


	void fakeDtor();
	void fakeCtor();

	int games;
	int mudul;
private:	
	static Manager* m_instance;
	Manager(void);

};



#endif