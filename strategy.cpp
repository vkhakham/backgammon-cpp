#include "strategy.h"
#include <assert.h>

#define INFINITY_VAL 2000

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
	double heurristic_val = 0;
	assert(node != NULL);
	if (!my_move)
		switch_board(node->m_board, node->mdlMe, node->mdlHim);
	heurristic_val = 0;
	heurristic_val -= calc_my_dist(node);
	heurristic_val += calc_op_dist(node);
	heurristic_val -= calc_my_jailed(node);
	heurristic_val += calc_op_jailed(node);
	heurristic_val += inhouse(node);
	heurristic_val += houses(node);
	heurristic_val += safe(node);
	heurristic_val -= open(node);
	heurristic_val += out(node);

	heurristic_val -= open_inhouse(node);
	heurristic_val -= blocked_moves(node);
	heurristic_val -= at_enemy_house(node);
	return heurristic_val;
}

// calculate my distance till win
double strategy::calc_my_dist(Node* node)
{
	assert(node != NULL);
	int* brd = node->m_board;
	int my_dist = 0;
	for (int i = 1; i < 25; i++)
	{
			my_dist += (25 - i) * brd[i];
	}

	my_dist += node->mdlMe * 26;

	if (my_dist == 0) return INFINITY_VAL;
	return my_dist;
}

// calculate my-dist-to-finish - his-dist-to-finish
double strategy::calc_op_dist(Node* node)
{
	assert(node != NULL);
	int* brd = node->m_board;
	int dist = 0;
	for (int i = 1; i < 25; i++)
	{
		dist += i * abs(brd[i]);
	}

	dist += node->mdlHim * 26;

	return dist;
}

//penalty for each own piece in jail * number of blocked entry places
//return positive value, higher -> worse
double strategy::calc_my_jailed(Node* node)
{
	int my_jail = node->mdlMe;
	int* brd = node->m_board;
	int blocked = 0;

	assert(node != NULL);
	if (my_jail > 0)
	{
		for (int i = 1; i < 7; i++)
		{
			if (brd[i] <= -2) blocked++;
		}
	}
	
	if (blocked > 4) blocked = 2;
	else blocked = 1;

	if (blocked == 6) return INFINITY_VAL;
	
	return my_jail * blocked *JAIL_MULTIPLIER;
}

//bonus for each opponent piece in jail * number of blocked entry places
// return positive value, higher -> better
double strategy::calc_op_jailed(Node* node)
{
	int his_jail = node->mdlHim;
	int* brd = node->m_board;
	int blocked = 0;

	assert(node != NULL);
	if (his_jail > 0)
	{
		blocked = 0;
		for (int i = 19; i < 25; i++)
		{
			if (brd[i] >= 2) blocked++;
		}
		his_jail *= blocked;
	}

	if (blocked == 6) return INFINITY_VAL;

	return his_jail * JAIL_MULTIPLIER;
}

//bonus for each piece in house
double strategy::inhouse(Node* node)
{
	int inhouse = 0;
	int* brd = node->m_board;
	
	assert(node != NULL);
	
	for (int i = 19; i < 25; i++)
	{
		if (brd[i] > 0) inhouse += brd[i];
	}

	return inhouse * INHOUSE_BONUS;
}

//bonus for each own blocked points on board
double strategy::houses(Node *node)
{
	int result = 0;
	int* brd = node->m_board;
	
	assert(node != NULL);
	
	for (int i = 1; i < 25; i++)
	{
		if (brd[i] >= 2)
		{
			result++;
			if (i >= 19) result += 0.5;
		}
	}
	return result * BLOCKED_BONUS;
}

//penalty for each own open piece on board that is still at risk of being eaten
double strategy::open(Node *node)
{
	int result = 0, temp_res = 0, blocked = 0;
	int* brd = node->m_board;
	
	assert(node != NULL);

	for (int i = 1; i < 25; i++)
	{
		if (brd[i] == 1) temp_res++;
		if (brd[i] < 0)
		{
			result += temp_res;
			temp_res = 0;
		}
	}

	for (int i = 1; i < 7; i++)
	{
		if (brd[i] <= -2)
		{
			blocked++;
		}
	}

	if (blocked <= 4) blocked = 1;
	else blocked = 2;

	return result * OPEN_MULTIPLIER * blocked;
}

//return bonus for each piece beyond enemy lines
double strategy::safe(Node *node)
{
	int result = 0;
	int* brd = node->m_board;
	assert(node != NULL);
	for (int i = 1; i < 25; i++)
	{
		if (brd[i] > 0) result += brd[i];
		if (brd[i] < 0) result = 0;
	}
	return result * SAFE_BONUS;
}

double strategy::out(Node *node)
{
	int in = 0;
	int* brd = node->m_board;
	assert(node != NULL);
	for (int i = 1; i < 25; i++)
	{
		if (brd[i] > 0) in += brd[i];
	}
	in += node->mdlMe;
	return (15 - in) * OUT_MULTIPLIER;
}

double strategy::open_inhouse(Node* node)
{
	int safe = 1, temp = 0, open = 0;
	int* brd = node->m_board;
	
	assert(node != NULL);
	
	for (int i = 1; i < 25; i++)
	{
		if (brd[i] > 0) temp++;
		if (brd[i] < 0 && temp > 0) safe = 0;
	}

	if (safe) return 0;

	for (int i = 19; i < 25; i++)
	{
		if (brd[i] == 1) open++;
	}

	return open * OPEN_INHOUSE_MULTIPLIER;
}

double strategy::blocked_moves(Node* node)
{
	int blocked = 0, result = 0;
	int* brd = node->m_board;

	assert(node != NULL);

	for (int i = 1; i < 25; i++)
	{
		if (brd[i] > 0)
		{
			for (int j = i; j < i + 7; j++)
			{
				if (brd[i] <= -2) blocked++;
			}
			if (blocked >= 3) result += blocked - 2;
		}
		blocked = 0;
	}
	return result * BLOCKED_MOVES;
}

double strategy::at_enemy_house(Node* node)
{
	int result = 0;
	int* brd = node->m_board;

	for (int j = 1; j < 4; j++)
	{
		if (brd[j] > 0) result += brd[j]*(7-j);
	}

	for (int j = 5; j < 7; j++)
	{
		if (brd[j] > 0) result += brd[j] * (7 - j) * 0.5;
	}
	return result * AT_ENEMY_HOUSE_MULTIPLIER;
}
