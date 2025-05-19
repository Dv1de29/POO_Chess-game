
#include "piece.h"

std::vector<Coord> King::getMoves(Coord from) {
    std::vector<Coord> moves;
    if ( from.col != 'a' ) {
        moves.push_back(Coord{static_cast<char>(from.col - 1), from.row});
        if ( from.row != 0 ) {
            moves.push_back(Coord{static_cast<char>(from.col - 1), from.row - 1});
        }
        if ( from.row != 7 ) {
            moves.push_back(Coord{static_cast<char>(from.col - 1), from.row + 1});
        }
    }
    if ( from.col != 'h') {
        moves.push_back(Coord{static_cast<char>(from.col + 1), from.row});
        if ( from.row != 0 ) {
            moves.push_back(Coord{static_cast<char>(from.col + 1), from.row - 1});
        }
        if ( from.row != 7 ) {
            moves.push_back(Coord{static_cast<char>(from.col + 1), from.row + 1});
        }
    }
    if ( from.row != 7 ) {
        moves.push_back(Coord{from.col, from.row + 1});
    }
    if ( from.row != 0 ) {
        moves.push_back(Coord{from.col, from.row - 1});
    }


    return moves;
}

std::vector<Coord> Queen::getMoves(Coord from) {
    std::vector<Coord> moves;
    return moves;
}

std::vector<Coord> Rook::getMoves(Coord from) {
    std::vector<Coord> moves;
    return moves;
}

std::vector<Coord> Bishop::getMoves(Coord from) {
    std::vector<Coord> moves;
    return moves;
}

std::vector<Coord> Knight::getMoves(Coord from) {
    std::vector<Coord> moves;

    if ( from.col != 'a' && from.col != 'b') {
        if ( from.row != 0 ) {
            moves.push_back(Coord{static_cast<char>(from.col - 2), from.row - 1});
        }
        if ( from.row != 7 ) {
            moves.push_back(Coord{static_cast<char>(from.col - 2), from.row + 1});
        }
    }
    if ( from.col != 'h'&& from.col != 'g') {
        if ( from.row != 7 ) {
            moves.push_back(Coord{static_cast<char>(from.col + 2), from.row + 1});
        }
        if ( from.row != 0 ) {
            moves.push_back(Coord{static_cast<char>(from.col + 2), from.row - 1});
        }
    }
    if ( from.row != 7 && from.row != 6 ) {
        if ( from.col != 'a' ) {
            moves.push_back(Coord{static_cast<char>(from.col - 1), from.row + 2});
        }
        if ( from.col != 'h' ) {
            moves.push_back(Coord{static_cast<char>(from.col + 1), from.row + 2});
        }
    }
    if ( from.row != 0 && from.row != 1 ) {
        if ( from.col != 'a' ) {
            moves.push_back(Coord{static_cast<char>(from.col - 1), from.row - 2});
        }
        if ( from.col != 'h' ) {
            moves.push_back(Coord{static_cast<char>(from.col + 1), from.row  - 2});
        }
    }

    return moves;
}

std::vector<Coord> Pawn::getMoves(Coord from) {
    std::vector<Coord> moves;

    if ( from.row == 1) {
        moves.push_back(Coord{from.col, 3});
    }
    moves.push_back(Coord{from.col, 2});

    return moves;
}

std::string King::type() const { return "King";}
std::string Queen::type() const { return "Queen";}
std::string Rook::type() const { return "Rook";}
std::string Bishop::type() const { return "Bishop";}
std::string Knight::type() const { return "Knight";}
std::string Pawn::type() const { return "Pawn";}

void Rook::setMoved() { this->Moved = true; }
const bool Rook::hasMoved() { return this->Moved; }



