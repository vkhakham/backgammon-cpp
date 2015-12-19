#ifndef __STRATEGY__
#define __STRATEGY__
#include "Player.h"

#define MIN_POS 0
#define MAX_POS 24

typedef struct
{
	int sum_dist;
} strategy_params;

class strategy
{
public:
	strategy(strategy_params params);
	/*TODO add to node class a variable to hold score*/
	void evaluate_tree(Node* node);
	void evaluate_node(Node* node);
private:
	strategy_params _params;
};

int calc_dist(Node* node);

#endif /*__STRATEGY__*/