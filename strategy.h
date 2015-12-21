#ifndef __STRATEGY__
#define __STRATEGY__

#include "Manager.h"
#include "Player.h"

#define MIN_POS 0
#define MAX_POS 24

class Node;

typedef struct
{
	int sum_dist;
} strategy_params;

class strategy
{
public:
	//strategy(/*strategy_params params*/);
	double evaluate_node(Node* node);
private:
	//strategy_params _params;
};

double calc_dist(Node* node);

#endif /*__STRATEGY__*/