//
// Created by David on 4/24/2025.
//

#ifndef HISTORY_H
#define HISTORY_H

#include "../Pieces/piece.h"

class BoardHistory {
    std::string moves = std::string("");
    int movesCount = 1;

   public:
     BoardHistory() = default;
     ~BoardHistory() = default;

     void addMove(const std::string& piece,const std::string& color, Coord from, Coord to, int castletype, bool needExplanation);
     std::string getMoves();
};



#endif //HISTORY_H
