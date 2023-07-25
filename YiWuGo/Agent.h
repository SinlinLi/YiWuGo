#pragma once


#include <random>
#include "Board.h"
#include "common.h"

enum PolicyType {
	RANDOM,
	HUMAN,
	AI,
};

class Agent
{
public:
	Player player;
	PolicyType policyType;
	mt19937 rng;

	Agent(Player player,PolicyType policyType);
	Agent();
	pii generateMove(Board board);
};

