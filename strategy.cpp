#include "strategy.h"
#include <assert.h>

#define INFINITY 1000

void switch_board(int* brd, int mdlMe, int mdlHim)
{
	for (int i = 1, j = 24; i < j; i++, j--)
	{
		int tmp = brd[i] * (-1);
		brd[i] = brd[j] * (-1);
		brd[j] = tmp;

		tmp = mdlMe;
		mdlMe = mdlHim;
		mdlHim = tmp;
	}
}

double strategy::evaluate_node(Node* node, bool my_move)
{
	assert(node != NULL);
	if (!my_move)
		switch_board(node->m_board, node->mdlMe, node->mdlHim);
	node->heurristic_val = 0;
	node->heurristic_val += calc_dist(node);
	node->heurristic_val += jail(node);
	node->heurristic_val += inhouse(node);
	node->heurristic_val += blocked(node);
	node->heurristic_val += safe(node);
	node->heurristic_val += open(node);
	return 0;
}

// calculte my-dist-to-finish - his-dist-to-finish
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

	dist += node->mdlHim * 25;
	my_dist += node->mdlHim * 25;

	if (my_dist == 0) return INFINITY;
	if (dist == 0) return -INFINITY;
	return dist + (375 - my_dist);
}

//penalty for each own piece in jail * number of blocked entry places
//bonus for each opponent piece in jail * number of blocked entry places
double strategy::jail(Node* node)
{
	int my_jail = node->mdlMe;
	int his_jail = node->mdlHim;
	int* brd = node->m_board;
	int blocked = 0;

	if (my_jail > 0)
	{
		for (int i = 1; i < 7; i++)
		{
			if (brd[i] <= -2) blocked++;
		}
		my_jail *= blocked;
	}

	if (his_jail > 0)
	{
		blocked = 0;
		for (int i = 19; i < 25; i++)
		{
			if (brd[i] >= 2) blocked++;
		}
		his_jail *= blocked;
	}
	
	return (his_jail - my_jail)*JAIL_MULTIPLIER;
}

//bonus for each piece in house
//penalty for each piece in house
double strategy::inhouse(Node* node)
{
	int inhouse = 0;
	int* brd = node->m_board;
	for (int i = 19; i < 25; i++)
	{
		if (brd[i] < 0) inhouse += brd[i];
	}

	return inhouse * INHOUSE_BONUS;
}

//bonus for each own blocked points on board
double strategy::blocked(Node *node)
{
	int result = 0;
	int* brd = node->m_board;
	for (int i = 1; i < 25; i++)
	{
		if (brd[i]>2) result++;
	}
	return result * BLOCKED_BONUS;
}

//penalty for each own open piece on board that is still at risk of being eaten
double strategy::open(Node *node)
{
	int result = 0, temp_res = 0;
	int* brd = node->m_board;
	for (int i = 1; i < 25; i++)
	{
		if (brd[i] > 0) temp_res += brd[i];
		if (brd[i] < 0)
		{
			result += temp_res;
			temp_res = 0;
		}
	}
	return result * OPEN_PENALTY;
}

//return bonus for each piece beyond enemy lines
double strategy::safe(Node *node)
{
	int result = 0;
	int* brd = node->m_board;
	for (int i = 1; i < 24; i++)
	{
		if (brd[i] > 0) result += brd[i];
		if (brd[i] < 0) result = 0;
	}
	return result * SAFE_BONUS;
}