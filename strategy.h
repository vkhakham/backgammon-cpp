#ifndef __STRATEGY__
#define __STRATEGY__

#include "Manager.h"
#include "Player.h"

#define MIN_POS 0
#define MAX_POS 24
#define JAIL_MULTIPLIER 4
#define SAFE_BONUS 0.5
#define BLOCKED_BONUS 1
#define OPEN_MULTIPLIER 15
#define INHOUSE_BONUS 1

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
	double strategy::calc_my_dist(Node* node);
	double strategy::calc_op_dist(Node* node);
	double calc_my_jailed(Node* node);
	double strategy::calc_op_jailed(Node* node);
	double strategy::inhouse(Node* node);
	double strategy::houses(Node *node);
	double strategy::safe(Node *node);
	double strategy::open(Node *node);
private:
	//strategy_params _params;
};


#endif /*__STRATEGY__*/