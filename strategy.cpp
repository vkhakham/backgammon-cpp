#include "strategy.h"
#include <assert.h>

double strategy::evaluate_node(Node* node, bool my_move)
{
	assert(node != NULL);
	if (!my_move)
		switch_board(node->m_board);
	double val = 0;
	val += calc_dist(node);
	val += jail(node, JAIL_MULTIPLIER, my_move);

	return val;
}

double strategy::calc_dist(Node* node)
{
	assert(node != NULL);
	int* brd = node->m_board;
	int dist = 0, my_dist = 0;
	for (int i = 1; i < 25; i++)
	{
		if (brd[i] < 0)
		{
			dist += i * brd[i];
		}
		else {
			my_dist += (25 - i) * brd[i];
		}
	}
	return dist + (375 - my_dist);
}

double strategy::jail(Node* node, double jail_penalty_multi, bool my_move)
{
	if (my_move)
		return ((node->mdlMe*(-1) + node->mdlHim) * jail_penalty_multi);
	else
		return ((node->mdlMe + node->mdlHim*(-1)) * jail_penalty_multi);

}