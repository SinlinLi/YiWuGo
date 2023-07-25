#include "SGFExporter.h"


SGFExporter::SGFExporter(int boardSize, float komi) : boardSize(boardSize), komi(komi) {
    sgfFile.open("output.sgf");
    sgfFile << "(;GM[1]FF[4]SZ[" << boardSize << "]KM[" << komi << "]";
}

void SGFExporter::exportMove(pii stone, Player player) {
    sgfFile << ";" << getColorString(player) << "[" << getCoordinateString(stone) << "]";
}

void SGFExporter::close() {
    sgfFile << ")";
    sgfFile.close();
}


string SGFExporter::getColorString(Player player) {
    return (player == BLACK) ? "B" : "W";
}

string SGFExporter::getCoordinateString(pii stone) {
    int y = stone.first;   //行列似乎要反一下
    int x = stone.second;
    char letter = 'a' + x;
    char number = 'a' + y;
    return { letter, number };
}