#ifndef __STRATEGY__
#define __STRATEGY__

#include "Manager.h"
#include "Player.h"

#define MIN_POS 0
#define MAX_POS 24
#define JAIL_MULTIPLIER 10
#define SAFE_BONUS 7
#define BLOCKED_BONUS 5
#define OPEN_PENALTY -12
#define INHOUSE_BONUS 3

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
	double jail(Node* node);
	double strategy::inhouse(Node* node);
	double strategy::blocked(Node *node);
	double strategy::safe(Node *node);
	double strategy::open(Node *node);
private:
	//strategy_params _params;
};


#endif /*__STRATEGY__*/