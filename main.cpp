#include "Manager.h"

int main()
{
	Manager::instance()->runGame();
	
	system("pause");
	return 0;
}

/*
todo:

V fill help func
V support all cin (endless loop with wrong input. just check input and if illegal reprint board and try again) -flush  ? getchar
V fill in validator correct msgs. upon calling print board, give the func the val->msg
X rule : haveTOPlay both  
V a func that recive all input ( Swap , Help , Give-up(are you sure?)->exit )
V signal game over - win

V do check of pieces left
V clear buffer
V dtor ??
V teko

V  timer for minimax
V  minimax with half moves
V  fix bug for getting to swapdice with 0 turns
V  fix bags for dices - convert to matrix
V support 2 version : two bags of 46 dices or 1 shared bag of 72 dices
 minimax alpha beta
V  support input from user : minimax or alpha-beta, maxDepth, player one/two is pc/human =>in rungame()
 support game vs another program with mutex
 finish heuristics grading
 booklet

*/