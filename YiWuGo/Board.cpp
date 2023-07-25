#include "Board.h"
	
#include <cassert>

int stoneBlockCount=0;

StoneBlock::StoneBlock(Player player, unordered_set<pii, PairHash> stones, unordered_set<pii, PairHash> qi):player(player),stones(stones),qi(qi)
{
}

StoneBlock* StoneBlock::merge(StoneBlock* a, StoneBlock* b)
{
	//assert(a->player == b->player);
	StoneBlock* c = create(a->player, a->stones, a->qi);
	c->stones.insert(b->stones.begin(), b->stones.end());
	c->qi.insert(b->qi.begin(), b->qi.end());
	for (pii i : c->stones) //删掉被占据的气 （待优化）
		c->qi.erase(i);

	return c;
}

Board::Board(int size):size(size)
{
	assert(size<=19&&size>=2);
	memset(stoneBlocks, 0, sizeof stoneBlocks);
}

bool Board::isOnBoard(pii stone)
{
	return stone.first >= 0 && stone.first < size && stone.second >= 0 && stone.second < size;
}

void Board::printBoard()
{

	char stoneChar[] = { '.','x','o' };
	//cout << endl;
	cout << ' ';
	for (int i = 0; i < size; i++)
	{
		cout << ' ' << COLS[i];
	}
	cout << nl;
	for (int i = 0; i < size; i++)
	{
		cout << ROWS[i];
		for (int j = 0; j < size; j++)
		{
			Player player;
			if (stoneBlocks[i][j] != nullptr)
			{
				player = stoneBlocks[i][j]->player;
			}
			else
			{
				player = NONE;
			}
			cout << ' ' << stoneChar[player];
		}
		cout << nl;
	}
	cout << endl;



	calcAvailable(whosTurn);
	cout << ' ';
	for (int i = 0; i < size; i++)
	{
		cout << ' ' << COLS[i];
	}
	cout << nl;
	for (int i = 0; i < size; i++)
	{
		cout << ROWS[i];
		for (int j = 0; j < size; j++)
		{
			if (whosTurn == BLACK)
			{
				cout << ' ' << isAvailableBlack[i][j];
			}
			else
			{
				cout << ' ' << isAvailableWhite[i][j];
			}
			
		}
		cout << nl;
	}
	cout << endl;
}

void Board::update(Player player, pii stone) //没有合法性检查
{
	lastMove = stone;
	whosTurn = player == BLACK ? WHITE : BLACK;
	step++;
	if (stone == make_pair(-1, -1))
	{
		zobristRecords.insert(make_pair(zobrist,player));
	}
	else
	{
		unordered_set<pii, PairHash> stones;
		stones.insert(stone);
		unordered_set<pii, PairHash> qi;
		set<StoneBlock*> sameBlock;
		set<StoneBlock*> oppositeBlock;
		for (int i = 0; i < 4; i++)
		{
			pii neighbourStone = make_pair(stone.first + dx[i], stone.second + dy[i]);
			if (isOnBoard(neighbourStone) == false) continue;
			auto stoneBlock = stoneBlocks[neighbourStone.first][neighbourStone.second];
			if (stoneBlock == nullptr)
			{
				qi.insert(neighbourStone);
			}
			else if (stoneBlock->player == player)
			{
				sameBlock.insert(stoneBlock);
			}
			else
			{
				oppositeBlock.insert(stoneBlock);
			}

		}
		StoneBlock* newStoneBlock = StoneBlock::create(player, stones, qi);
		updateZobrist(player, stone);
		//zobrist
		for (auto i : sameBlock)
		{
			//assert(false);
			auto oldStoneBlock = newStoneBlock;
			newStoneBlock = StoneBlock::merge(newStoneBlock, i);
			StoneBlock::remove(i);
			StoneBlock::remove(oldStoneBlock);

		}
		for (auto i : newStoneBlock->stones)
		{
			stoneBlocks[i.first][i.second] = newStoneBlock; //更新指针
		}

		for (auto i : oppositeBlock)
		{

			i->qi.erase(stone);
			//cout << qi.size()<<endl<<endl;
			if (i->qi.size() == 0)
			{
				//assert(false);

				for (auto j : i->stones)
				{
					for (int k = 0; k < 4; k++)
					{
						pii neighbourStone = make_pair(j.first + dx[k], j.second + dy[k]);
						if (isOnBoard(neighbourStone) == false) continue;
						StoneBlock* stoneBlock = stoneBlocks[neighbourStone.first][neighbourStone.second];
						if (stoneBlock != nullptr && stoneBlock->player == player)
						{
							stoneBlocks[neighbourStone.first][neighbourStone.second]->qi.insert(j);
						}

						//zobrist;
					}
					//cout << "11111111111111111111111";
					stoneBlocks[j.first][j.second] = nullptr;
					updateZobrist(player == BLACK ? WHITE: BLACK, j);
				}
				StoneBlock::remove(i);
				
			}
		}
		zobristRecords.insert(make_pair(zobrist, player));
	}
}

bool Board::emulate(Player player, pii stone)
{
	unordered_set<pii, PairHash> stones;
	stones.insert(stone);
	unordered_set<pii, PairHash> qi;
	set<StoneBlock*> sameBlock;
	set<StoneBlock*> oppositeBlock;
	for (int i = 0; i < 4; i++)
	{
		pii neighbourStone = make_pair(stone.first + dx[i], stone.second + dy[i]);
		if (isOnBoard(neighbourStone) == false) continue;
		auto stoneBlock = stoneBlocks[neighbourStone.first][neighbourStone.second];
		if (stoneBlock == nullptr)
		{
			return true;
		}
		else if (stoneBlock->player == player)
		{
			sameBlock.insert(stoneBlock);
		}
		else
		{
			if (stoneBlock->qi.size() == 1 && stoneBlock->qi.count(stone) == 1)
			{
				qi.insert(neighbourStone);
				oppositeBlock.insert(stoneBlock);  //存会被吃掉的棋
			}

		}
	}
	for (auto i : oppositeBlock)
	{
		for (auto j : i->stones)
		{
			updateZobrist(player == BLACK ? WHITE: BLACK, j); //模拟移除棋子
		}
	}
	StoneBlock* newStoneBlock = StoneBlock::create(player, stones, qi);
	//zobrist
	for (auto i : sameBlock)
	{
		auto oldStoneBlock = newStoneBlock;
		newStoneBlock = StoneBlock::merge(newStoneBlock, i);
		StoneBlock::remove(oldStoneBlock);
	}
	if (newStoneBlock->qi.size() == 0) //如果无气 结束
	{
		StoneBlock::remove(newStoneBlock);
		for (auto i : oppositeBlock)
		{
			for (auto j : i->stones)
			{
				updateZobrist(player == BLACK ? WHITE : BLACK, j); //撤销
			}
		}
		return false;
	}
	updateZobrist(player, stone); //落子哈希
	if (zobristRecords.count(make_pair(zobrist, player)))
	{
		//撤销
		updateZobrist(player, stone); 
		for (auto i : oppositeBlock)
		{
			for (auto j : i->stones)
			{
				updateZobrist(player == BLACK ? WHITE : BLACK, j);
			}
		}
		StoneBlock::remove(newStoneBlock);
		//撤销
		return false;
	}
	//撤销
	updateZobrist(player, stone); 
	for (auto i : oppositeBlock)
	{
		for (auto j : i->stones)
		{
			updateZobrist(player == BLACK ? WHITE : BLACK, j);
		}
	}
	StoneBlock::remove(newStoneBlock);
	//撤销
	return true;

}

void Board::updateZobrist(Player player, pii stone)
{
	zobrist ^= ZOBRIST_CODE[player][stone.first][stone.second];
}

bool Board::isLegalMove(Player player, pii stone) //可以落子
{
	if (stone == make_pair(-1, -1))
	{
		return true;
	}
	if (!isOnBoard(stone))
	{
		return false;
	}
	if (stoneBlocks[stone.first][stone.second] != nullptr)
	{
		return false;
	}
	if (!emulate(player, stone))
		return false;
	return true;
} //可以落子

bool Board::isLegalPolicy(Player player, pii stone) //不填眼的落子
{
	if (!isLegalMove(player,stone)) return false;
	unordered_set<pii, PairHash> neighbourStones;
	for (int i = 0; i < 4; i++)
	{
		pii neighbourStone = make_pair(stone.first + dx[i], stone.second + dy[i]);
		if (isOnBoard(neighbourStone) == false) continue;
		auto stoneBlock = stoneBlocks[neighbourStone.first][neighbourStone.second];
		if (stoneBlock == nullptr || stoneBlock->player != player) //不是眼
		{
			return true;
		}
		neighbourStones.insert(neighbourStone);
	}
	pii neighbourStone = *neighbourStones.begin();
	StoneBlock * stoneBlock = stoneBlocks[neighbourStone.first][neighbourStone.second];
	for (auto i: neighbourStones)
	{
		if (!stoneBlock->stones.count(i)) //如果不是子集
		{
			return true; //说明不是眼
		}
	}
	return false;
}

bool Board::calcAvailable(Player player)
{
	bool flag = false;
	for (int i = 0; i < size; i++)
	{
		for (int j=0;j< size;j++)
		{
			if (player == BLACK)
			{
				isAvailableBlack[i][j]=isLegalPolicy(player,make_pair(i, j));
				if (isAvailableBlack[i][j]) flag = true;
			}
			else
			{
				isAvailableWhite[i][j] = isLegalPolicy(player, make_pair(i, j));
				if (isAvailableWhite[i][j]) flag = true;
			}
		}
	}
	return flag;
}

