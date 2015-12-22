#ifndef __STRATEGY__
#define __STRATEGY__

#include "Manager.h"
#include "Player.h"

#define MIN_POS 0
#define MAX_POS 24
#define JAIL_MULTIPLIER 40

class Node;


typedef struct
{
	int sum_dist;
} strategy_params;

class strategy
{
public:
	//strategy(/*strategy_params params*/);
	double evaluate_node(Node* node, bool my_move);
	double calc_dist(Node* node);
	double jail(Node* node, double jail_penalty_multi, bool my_move);
private:
	//strategy_params _params;
};


#endif /*__STRATEGY__*/