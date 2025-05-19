
#include "history.h"

void BoardHistory::addMove(const std::string& piece,const std::string& color, Coord from, Coord to, int castletype, bool needExplanation) {
    if ( color == "white" ) {
        moves += "\n\n" + std::to_string(this->movesCount) + ".";
    } else {
        moves += "    ";
        movesCount += 1;
    }

    if ( castletype == 1 ) {
        moves += "O-O";
        return;
    }
    if ( castletype == 2 ) {
        moves += "O-O-O";
        return;
    }

    if ( piece == "King" ) {
        moves += "K";
    } else if ( piece == "Knight" ) {
        moves += "N";
    } else if ( piece == "Bishop" ) {
        moves += "B";
    } else if ( piece == "Queen" ) {
        moves += "Q";
    } else if ( piece == "Rook" ) {
        moves += "R";
    }

    if ( needExplanation ) {
        if ( from.col != to.col ) {
            moves += from.col;
        } else {
            moves += std::to_string(from.row + 1);
        }
    }

    moves += to.col + std::to_string(to.row + 1);
    if ( piece == "Pawn" ) {
        moves += "  ";
    }
}

std::string BoardHistory::getMoves() { return this->moves; }

