#include "strategy.h"
#include <assert.h>
strategy::strategy(strategy_params params)
{
	_params = params;
}

int calc_dist(Node* node)
{
	assert(node != NULL);
	int* brd = node->m_board;
	int dist = 0;
	for (int i = 1; i < 25; i++)
	{
		if (brd[i] < 0)
		{
			dist += (25 - i) * brd[i];
		}
		else {
			dist += i * brd[i];
		}
	}
	return dist;
}
