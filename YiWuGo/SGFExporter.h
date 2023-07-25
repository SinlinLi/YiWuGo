#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "common.h"

class SGFExporter {
public:

    int boardSize;
    float komi;
    std::ofstream sgfFile;

    SGFExporter(int boardSize, float komi);

    void exportMove(pii stone, Player player);
    void close();
    string getColorString(Player player);
    string getCoordinateString(pii stone);
};


