#include "Game.h"
#include "SGFExporter.h"

Game::Game(Agent agentBlack, Agent agentWhite, Board board, float komi ): agentBlack(agentBlack),agentWhite(agentWhite),board(board),komi(komi),exporter(board.size, komi)
{
}


void Game::nextState()
{
	if (!board.calcAvailable(board.whosTurn)) //Ëã¿ÉÂä×Óµã
	{
		float ans= calcResult();
		gameState = ans> 0 ? BLACK_WIN : WHITE_WIN;
		cout << (gameState == BLACK_WIN ? "ºÚÊ¤" : "°×Ê¤") << ans << endl;
		exporter.close();
	}
	if (gameState == RUNNING)
	{
		pii stone;
		do {
			if (board.whosTurn == BLACK)
			{

				stone = agentBlack.generateMove(board);

			}
			else
			{
				stone = agentWhite.generateMove(board);
			}
		} while (!board.isLegalPolicy(board.whosTurn,stone));

		exporter.exportMove(stone, board.whosTurn);
		board.update(board.whosTurn, stone);
		
	}
}

float Game::calcResult()
{
	int blackScore=0;
	int whiteScore=0;
	for (int i = 0; i < board.size; i++)
	{
		for (int j = 0; j < board.size; j++)
		{
			if (board.stoneBlocks[i][j] != nullptr)
			{
				if (board.stoneBlocks[i][j]->player == BLACK)
				{
					blackScore++;
				}
				else
				{
					whiteScore++;
				}
			}
		}
	}
	return blackScore - whiteScore - komi;
}