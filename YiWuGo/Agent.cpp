#include "Agent.h"
#include "common.h"

Agent::Agent(Player player, PolicyType policyType)
{
	this->player = player;
	this->policyType = policyType;
	rng.seed(std::random_device()());
}
Agent::Agent()
{

}
pii Agent::generateMove(Board board)
{
	switch (policyType)
	{
	case RANDOM:

		int x, y;
		x = rng() % board.size;
		y = rng() % board.size;
		return make_pair(x, y);
	case HUMAN:
		char a, b;
		char op;
		do
		{
			cout << "输入落子位置：";
			cin >> op;
			
			if (op == '/')
			{
				cin >> a >> b;
			}
			else
			{
				a = op;
				cin >> b;
			}
			assert(a >= 'A' && a <= 'T' && b >= 'A' && b <= 'T');
			if (op == '/')
			{
				StoneBlock* stoneBlock = board.stoneBlocks[a - 'A'][b - 'A'];
				if (stoneBlock == nullptr) continue;
				cout<<"棋子：";
				for (auto i : stoneBlock->stones)
				{
					cout << i;
				}
				cout << endl;
				cout << "气：";
				for (auto i : stoneBlock->qi)
				{
					cout << i;
				}
				cout << endl;
				
			}
		} while (op == '/');

		return make_pair(a - 'A', b - 'A');
	default:
		assert(false);
		return make_pair(-1, -1);
		break;

	}
}