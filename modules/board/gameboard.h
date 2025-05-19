
#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include "../Pieces/piece.h"
#include "../History/history.h"

class GameBoard {
    BoardHistory boardHistory;

  std::vector<std::vector<std::unique_ptr<Piece>>> pieces;

    struct Move {
        Coord from, to;
    };

    struct pos {
        int x,y;
    }pos_;

    struct size {
        int w,h;
    }size_;

    Coord currentCoord = {'e',1};
    Coord selectedCoord;
    bool selectedBox = false;
    std::string turn = "white";

    bool whiteKingMoved = false;
    bool blackKingMoved = false;

public:
   GameBoard() : pieces([] {
     std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
     for ( auto& row : grid) {
       row.resize(8);
     }
     return grid;
   }() ) {
    for ( int i = 0; i <= 7; i++ ) {
      pieces[1][i] = std::make_unique<Pawn>("white");
      pieces[6][i] = std::make_unique<Pawn>("black");

      switch (i) {
        case 0:
        case 7:
            pieces[0][i] = std::make_unique<Rook>("white");
            pieces[7][i] = std::make_unique<Rook>("black");
        break;
        case 1:
        case 6:
            pieces[0][i] = std::make_unique<Knight>("white");
            pieces[7][i] = std::make_unique<Knight>("black");
        break;
        case 2:
        case 5:
            pieces[0][i] = std::make_unique<Bishop>("white");
            pieces[7][i] = std::make_unique<Bishop>("black");
        break;
        case 3:
            pieces[0][i] = std::make_unique<Queen>("white");
            pieces[7][i] = std::make_unique<Queen>("black");
        break;
        case 4:
            pieces[0][i] = std::make_unique<King>("white");
            pieces[7][i] = std::make_unique<King>("black");
        break;
        default:
          break;
      }
    }
  };
  ~GameBoard() = default;

  GameBoard(const GameBoard&) = delete;
  GameBoard& operator=(const GameBoard&) = delete;
  GameBoard(GameBoard&&) noexcept = default;
  GameBoard& operator=(GameBoard&&) noexcept = default;

    std::unique_ptr<Piece>& getPiece(int row, int col);
    void setSize(int w, int h);

    void setPos(int x, int y);
    bool inBoard( int x, int y);

    void setCoord( char col, int row);
    void setCoordRow( int row);
    void setCoordCol( char col);
    Coord getCoord();

    void setSelectedCoord( char col, int row);
    Coord getSelectedCoord();

    void setSelectedBox( bool selected);
    bool getSelectedBox();

    void setTurn( std::string turn);
    std::string getTurn();

    bool movePiece(Coord from, Coord to);

    std::string getMovesString();

    Coord getCoordForMouse(int x, int y);

    std::vector<Coord> legalMoves(Coord from, bool skipKingSafety = false);
    Coord findKing(std::string color);
    const bool isSafeMove(Coord from, Coord to);
    const bool inCheck();
    const bool isCheckMate();
    std::vector<Coord> checkCastle();
};



#endif //GAMEBOARD_H
