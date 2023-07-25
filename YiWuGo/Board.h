#pragma once

#include <algorithm>
#include <unordered_set>
#include <set>
#include <vector>
#include <utility>
#include <cassert>
#include <string>
#include <iostream>

#include "common.h"
#include "HASH_CODE.h"

using namespace std;

const int dx[4] = { 1,0,-1,0 };
const int dy[4] = { 0,1,0,-1 };
const string COLS = "ABCDEFGHIJKLMNOPQRST";
const string ROWS = "ABCDEFGHIJKLMNOPQRST";

extern int stoneBlockCount;

class StoneBlock
{
public:
	Player player;
	unordered_set<pii, PairHash> stones;
	unordered_set<pii, PairHash > qi;
	
	StoneBlock(Player player, unordered_set<pii, PairHash> stones, unordered_set<pii, PairHash> qi);
	static StoneBlock* merge(StoneBlock* a, StoneBlock* b);



	inline static StoneBlock* create(Player player, unordered_set<pii, PairHash> stones, unordered_set<pii, PairHash> qi)
	{
		stoneBlockCount++;
		return new StoneBlock(player, stones, qi);
	}

	inline static void remove(StoneBlock* stoneBlock)
	{
		stoneBlockCount--;
		delete(stoneBlock);
		return;
	}
};

class Board
{
public:
	int size = 0;
	int step = 0;
	pii lastMove = make_pair(-1, -1);
	Player whosTurn = BLACK;
	StoneBlock* stoneBlocks[19][19];
	bool isAvailableBlack[19][19];
	bool isAvailableWhite[19][19];


	Board(int size);

	bool isOnBoard(pii stone);
	void printBoard();
	void update(Player player, pii stone);
	bool emulate(Player player, pii stone);
	void updateZobrist(Player player, pii stone);
	bool isLegalMove(Player player, pii stone);
	bool isLegalPolicy(Player, pii stone);
	bool calcAvailable(Player);

private:
	ull zobrist = EMPTY_BOARD;
	set<pair<ull, Player>> zobristRecords;
};

