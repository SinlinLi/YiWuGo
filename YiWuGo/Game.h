#pragma once
#include "Agent.h"
#include "Board.h"
#include "SGFExporter.h"

enum GameState
{
	RUNNING,
	BLACK_WIN,
	WHITE_WIN
};

class Game
{
public:
	Agent agentBlack;
	Agent agentWhite;
	Board board;
	GameState gameState = RUNNING;
	float komi = 7.5;
	SGFExporter exporter;

	Game(Agent agentBlack, Agent agentWhite, Board board, float komi=7.5);

	float calcResult(); //ËãÄ¿²î

	void nextState();
};

