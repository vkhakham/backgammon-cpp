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
*/