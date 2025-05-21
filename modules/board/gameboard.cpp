#include "gameboard.h"

std::unique_ptr<Piece>& GameBoard::getPiece(int row, int col) {
    return pieces[row][col];
}

void GameBoard::setSize(int w, int h) {
    this->size_ = {w, h};
}

void GameBoard::setPos(int x, int y) {
    this->pos_ = {x, y};
}

bool GameBoard::inBoard(int x, int y) {
    if ( pos_.x > x || pos_.x + size_.w < x || pos_.y > y || pos_.y + size_.h < y ) return false;
    return true;
}

void GameBoard::setCoord(char col, int row) {
    this->currentCoord = {col, row};
}

void GameBoard::setCoordRow(int row) {
    this->currentCoord.row = row;
}

void GameBoard::setCoordCol(char col) {
    this->currentCoord.col = col;
}

void GameBoard::setSelectedCoord(char col, int row) {
    this->selectedCoord = {col, row};
}

Coord GameBoard::getSelectedCoord() {
    return selectedCoord;
}

void GameBoard::setSelectedBox(bool selected) {
    this->selectedBox = selected;
}

bool GameBoard::getSelectedBox() {
    return this->selectedBox;
}

void GameBoard::setTurn(std::string turn) { this->turn = std::move(turn); }

std::string GameBoard::getTurn() { return this->turn; }

Coord GameBoard::getCoord() {
    return currentCoord;
}

std::string GameBoard::getMovesString() {
    return this->boardHistory.getMoves();
}

Coord GameBoard::getCoordForMouse(int x, int y) {
    float chessBoxsize = static_cast<float>(size_.w) / 8;
    return Coord{ static_cast<char>(static_cast<int>((x - pos_.x) / chessBoxsize) + 'a'), 7 - static_cast<int>((y - pos_.y) / chessBoxsize)};
}

std::vector<Coord> GameBoard::legalMoves(Coord from, bool skipKingSafety) {
    std::vector<Coord> moves;

    auto &piece = this->getPiece(from.row, static_cast<int>(from.col - 'a'));
    if (piece->type() == "King") {
        if (from.col != 'a') {
            moves.push_back(Coord{static_cast<char>(from.col - 1), from.row});
            if (from.row != 0) {
                moves.push_back(Coord{static_cast<char>(from.col - 1), from.row - 1});
            }
            if (from.row != 7) {
                moves.push_back(Coord{static_cast<char>(from.col - 1), from.row + 1});
            }
        }
        if (from.col != 'h') {
            moves.push_back(Coord{static_cast<char>(from.col + 1), from.row});
            if (from.row != 0) {
                moves.push_back(Coord{static_cast<char>(from.col + 1), from.row - 1});
            }
            if (from.row != 7) {
                moves.push_back(Coord{static_cast<char>(from.col + 1), from.row + 1});
            }
        }
        if (from.row != 7) {
            moves.push_back(Coord{from.col, from.row + 1});
        }
        if (from.row != 0) {
            moves.push_back(Coord{from.col, from.row - 1});
        }

        for (auto it = moves.begin(); it != moves.end();) {
            if (auto *isPiece = this->getPiece(it->row, static_cast<int>(it->col - 'a')).get();
                isPiece && isPiece->getColor() == piece->getColor()) {
                it = moves.erase(it);
            } else {
                if ( skipKingSafety == false && !this->isSafeMove(from, *it)) {
                    it = moves.erase(it);
                }
                else
                it++;
            }
        }

        if ( skipKingSafety == false ) {
            std::vector<Coord> castlemoves = this->checkCastle();
            for ( auto move : castlemoves ) {
                moves.push_back(move);
            }
        }

        return moves;
    }
    if (piece->type() == "Queen") {
        for ( int i = 1; from.row + i < 8; i++) {
            if ( auto* ispiece = pieces[from.row + i][static_cast<int>(from.col - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{from.col, from.row + i});
                }
                break;
            }
            moves.push_back(Coord{from.col, from.row + i});
        }
        for ( int i = 1; from.row - i >= 0; i++) {
            if ( auto* ispiece = pieces[from.row - i][static_cast<int>(from.col - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{from.col, from.row - i});
                }
                break;
            }
            moves.push_back(Coord{from.col, from.row - i});
        }
        for ( int i = 1; static_cast<char>(from.col + i) <= 'h'; i++) {
            if ( auto* ispiece = pieces[from.row][static_cast<int>(from.col + i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col + i), from.row});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col + i), from.row});
        }
        for ( int i = 1; static_cast<char>(from.col - i) >= 'a'; i++) {
            if ( auto* ispiece = pieces[from.row][static_cast<int>(from.col - i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col - i), from.row});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col - i), from.row});
        }

        for ( int i = 1; from.row + i < 8 && static_cast<char>(from.col + i) <= 'h'; i++) {
            if ( auto* ispiece = pieces[from.row + i][static_cast<int>(from.col + i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col + i), from.row + i});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col + i), from.row + i});
        }

        for ( int i = 1; from.row - i >= 0 && static_cast<char>(from.col + i) <= 'h'; i++) {
            if ( auto* ispiece = pieces[from.row - i][static_cast<int>(from.col + i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col + i), from.row - i});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col + i), from.row - i});
        }

        for ( int i = 1; from.row + i < 8 && static_cast<char>(from.col - i) >= 'a'; i++) {
            if ( auto* ispiece = pieces[from.row + i][static_cast<int>(from.col - i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col - i), from.row + i});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col - i), from.row + i});
        }

        for ( int i = 1; from.row - i >= 0 && static_cast<char>(from.col - i) >= 'a'; i++) {
            if ( auto* ispiece = pieces[from.row - i][static_cast<int>(from.col - i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col - i), from.row - i});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col - i), from.row - i});
        }

        for ( auto it = moves.begin(); it != moves.end(); ) {
            if ( skipKingSafety == false && !isSafeMove(from, *it)) {
                it = moves.erase(it);
            }
            else {
                ++it;
            }
        }

        return moves;
    }
    if (piece->type() == "Rook") {
        for ( int i = 1; from.row + i < 8; i++) {
            if ( auto* ispiece = pieces[from.row + i][static_cast<int>(from.col - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{from.col, from.row + i});
                }
                break;
            }
            moves.push_back(Coord{from.col, from.row + i});
        }
        for ( int i = 1; from.row - i >= 0; i++) {
            if ( auto* ispiece = pieces[from.row - i][static_cast<int>(from.col - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{from.col, from.row - i});
                }
                break;
            }
            moves.push_back(Coord{from.col, from.row - i});
        }
        for ( int i = 1; static_cast<char>(from.col + i) <= 'h'; i++) {
            if ( auto* ispiece = pieces[from.row][static_cast<int>(from.col + i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col + i), from.row});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col + i), from.row});
        }
        for ( int i = 1; static_cast<char>(from.col - i) >= 'a'; i++) {
            if ( auto* ispiece = pieces[from.row][static_cast<int>(from.col - i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col - i), from.row});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col - i), from.row});
        }

        for ( auto it = moves.begin(); it != moves.end(); ) {
            if ( skipKingSafety == false && !isSafeMove(from, *it)) {
                it = moves.erase(it);
            }
            else {
                ++it;
            }
        }

        return moves;
    }
    if (piece->type() == "Bishop") {
        for ( int i = 1; from.row + i < 8 && static_cast<char>(from.col + i) <= 'h'; i++) {
            if ( auto* ispiece = pieces[from.row + i][static_cast<int>(from.col + i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col + i), from.row + i});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col + i), from.row + i});
        }

        for ( int i = 1; from.row - i >= 0 && static_cast<char>(from.col + i) <= 'h'; i++) {
            if ( auto* ispiece = pieces[from.row - i][static_cast<int>(from.col + i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col + i), from.row - i});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col + i), from.row - i});
        }

        for ( int i = 1; from.row + i < 8 && static_cast<char>(from.col - i) >= 'a'; i++) {
            if ( auto* ispiece = pieces[from.row + i][static_cast<int>(from.col - i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col - i), from.row + i});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col - i), from.row + i});
        }

        for ( int i = 1; from.row - i >= 0 && static_cast<char>(from.col - i) >= 'a'; i++) {
            if ( auto* ispiece = pieces[from.row - i][static_cast<int>(from.col - i - 'a')].get(); ispiece) {
                if ( ispiece->getColor() != piece->getColor() ) {
                    moves.push_back(Coord{static_cast<char>(from.col - i), from.row - i});
                }
                break;
            }
            moves.push_back(Coord{static_cast<char>(from.col - i), from.row - i});
        }

        for ( auto it = moves.begin(); it != moves.end(); ) {
            if ( skipKingSafety == false && !isSafeMove(from, *it)) {
                it = moves.erase(it);
            }
            else {
                ++it;
            }
        }

        return moves;
    }
    if (piece->type() == "Knight") {
        if (from.col != 'a' && from.col != 'b') {
            if (from.row != 0) {
                moves.push_back(Coord{static_cast<char>(from.col - 2), from.row - 1});
            }
            if (from.row != 7) {
                moves.push_back(Coord{static_cast<char>(from.col - 2), from.row + 1});
            }
        }
        if (from.col != 'h' && from.col != 'g') {
            if (from.row != 7) {
                moves.push_back(Coord{static_cast<char>(from.col + 2), from.row + 1});
            }
            if (from.row != 0) {
                moves.push_back(Coord{static_cast<char>(from.col + 2), from.row - 1});
            }
        }
        if (from.row != 7 && from.row != 6) {
            if (from.col != 'a') {
                moves.push_back(Coord{static_cast<char>(from.col - 1), from.row + 2});
            }
            if (from.col != 'h') {
                moves.push_back(Coord{static_cast<char>(from.col + 1), from.row + 2});
            }
        }
        if (from.row != 0 && from.row != 1) {
            if (from.col != 'a') {
                moves.push_back(Coord{static_cast<char>(from.col - 1), from.row - 2});
            }
            if (from.col != 'h') {
                moves.push_back(Coord{static_cast<char>(from.col + 1), from.row - 2});
            }
        }

        for (auto it = moves.begin(); it != moves.end();) {
            if (auto *isPiece = this->getPiece(it->row, static_cast<int>(it->col - 'a')).get();
                isPiece && isPiece->getColor() == piece->getColor()) {
                it = moves.erase(it);
            } else {
                if ( skipKingSafety == false && !isSafeMove(from, *it)) {
                    it = moves.erase(it);
                }
                else
                it++;
            }
        }

        return moves;
    }
    if (piece->type() == "Pawn") {
        if (this->turn == "white") {
            if ( from.row != 7 ) {
                if (pieces[from.row + 1][static_cast<int>(from.col - 'a')] == nullptr) {
                    moves.push_back(Coord{from.col, from.row + 1});
                }
                if (from.row == 1) {
                    if (pieces[from.row + 2][static_cast<int>(from.col - 'a')] == nullptr) {
                        moves.push_back(Coord{from.col, from.row + 2});
                    }
                }
                if ( from.col != 'a' ) {
                    if ( auto* ispiece = this->pieces[from.row + 1][static_cast<int>(from.col - 1 - 'a')].get();
                        ispiece && ispiece->getColor() != piece->getColor()) {
                        moves.push_back(Coord{static_cast<char>(from.col - 1), from.row + 1});
                        }
                }
                if ( from.col != 'h' ) {
                    if ( auto* ispiece = this->pieces[from.row + 1][static_cast<int>(from.col + 1 - 'a')].get();
                        ispiece && ispiece->getColor() != piece->getColor()) {
                        moves.push_back(Coord{static_cast<char>(from.col + 1), from.row + 1});
                        }
                }
            }
        } else {
            if ( from.row != 0) {
                if (pieces[from.row - 1][static_cast<int>(from.col - 'a')] == nullptr) {
                    moves.push_back(Coord{from.col, from.row - 1});
                }
                if (from.row == 6) {
                    if (pieces[from.row - 2][static_cast<int>(from.col - 'a')] == nullptr) {
                        moves.push_back(Coord{from.col, from.row - 2});
                    }
                }
                if ( from.col != 'a' ) {
                    if ( auto* ispiece = this->pieces[from.row - 1][static_cast<int>(from.col - 1 - 'a')].get();
                        ispiece && ispiece->getColor() != piece->getColor()) {
                        moves.push_back(Coord{static_cast<char>(from.col - 1), from.row - 1});
                        }
                }
                if ( from.col != 'h' ) {
                    if ( auto* ispiece = this->pieces[from.row - 1][static_cast<int>(from.col + 1 - 'a')].get();
                        ispiece && ispiece->getColor() != piece->getColor()) {
                        moves.push_back(Coord{static_cast<char>(from.col + 1), from.row - 1});
                        }
                }
            }
        }

        for ( auto it = moves.begin(); it != moves.end(); ) {
            if ( skipKingSafety == false && !isSafeMove(from, *it)) {
                it = moves.erase(it);
            }
            else {
                ++it;
            }
        }

        return moves;
    }
}

bool GameBoard::movePiece(Coord from, Coord to) {
    bool isCastle = false;
    auto& piece = this->pieces[from.row][static_cast<int>(from.col - 'a')];
    if ( auto* topiece = pieces[to.row][static_cast<int>(to.col - 'a')].get();
        topiece && topiece->getColor() == piece->getColor()) {
        throw std::runtime_error("Another piece at the to position of the same color");
        }
    if ( auto* king = dynamic_cast<King*>(piece.get()); king) {
        if ( std::abs(static_cast<int>(from.col - to.col)) > 1 ) {
            isCastle = true;
        }
        if ( this->getTurn() == "white") {
            this->whiteKingMoved = true;
        }
        else {
            this->blackKingMoved = true;
        }
    }
    else if (auto* rook = dynamic_cast<Rook*>(piece.get()); rook) {
        rook->setMoved();
    }
    else if ( auto* pawn = dynamic_cast<Pawn*>(piece.get()); pawn) {
        if ( pawn->getColor() == "white" ) {
            if ( to.row == 7 ) {
                pieces[from.row][static_cast<int>(from.col - 'a')] = std::make_unique<Queen>("white");
            }
        }
        else {
            if ( to.row == 0 ) {
                pieces[from.row][static_cast<int>(from.col - 'a')] = std::make_unique<Queen>("black");
            }
        }
    }
    if (piece == nullptr) {
        throw std::runtime_error("Piece is null form coordinates" + from.col + from.row);
    }
    if ( isCastle == false ) {
        this->boardHistory.addMove(piece->type(), piece->getColor(), from, to, 0, false);
        pieces[to.row][static_cast<int>(to.col - 'a')] = std::move(piece);
    }
    else {
        if ( to.col == 'g'){
            this->boardHistory.addMove(piece->type(), piece->getColor(), from, to, 1, false);
            pieces[to.row][(to.col - 'a')] = std::move(piece);
            pieces[to.row][('f' - 'a')] = std::move(pieces[from.row][('h' - 'a')]);
        }
        else {
            this->boardHistory.addMove(piece->type(), piece->getColor(), from, to, 2, false);
            pieces[to.row][(to.col - 'a')] = std::move(piece);
            pieces[to.row][('d' - 'a')] = std::move(pieces[from.row][0]);
        }
    }

    return true;
}

const bool GameBoard::isSafeMove(Coord from, Coord to) {
    auto& currentPiece = this->pieces[from.row][static_cast<int>(from.col - 'a')];
    std::unique_ptr<Piece> toPiece = std::move(this->pieces[to.row][static_cast<int>(to.col - 'a')]);

    this->pieces[to.row][static_cast<int>(to.col - 'a')] = std::move(currentPiece);
    this->pieces[from.row][static_cast<int>(from.col - 'a')] = nullptr;

    Coord kingPos = this->findKing(this->getTurn());

    this->setTurn( this->getTurn() == "white" ? "black" : "white");
    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {
            if ( auto* piece = this->pieces[i][j].get();
                piece && piece->getColor() == this->getTurn()) {
                std::vector<Coord> moves = this->legalMoves(Coord{static_cast<char>(j + 'a'), i}, true);
                for ( auto move : moves) {
                    if ( move == kingPos) {
                        this->pieces[from.row][static_cast<int>(from.col - 'a')] = std::move(this->pieces[to.row][static_cast<int>(to.col - 'a')]);
                        this->pieces[to.row][static_cast<int>(to.col - 'a')] = std::move(toPiece);
                        this->setTurn( this->getTurn() == "white" ? "black" : "white");
                        return false;
                    }
                }
            }
        }
    }
    this->pieces[from.row][static_cast<int>(from.col - 'a')] = std::move(this->pieces[to.row][static_cast<int>(to.col - 'a')]);
    this->pieces[to.row][static_cast<int>(to.col - 'a')] = std::move(toPiece);
    this->setTurn( this->getTurn() == "white" ? "black" : "white");
    return true;
}

Coord GameBoard::findKing(std::string color) {
    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {
            if ( auto* piece = this->pieces[i][j].get();
                piece
                && piece->getColor() == color
                && piece->type() == "King") {
                return Coord{static_cast<char>(j + 'a'), i};
            }
        }
    }
}

const bool GameBoard::isCheckMate() {
    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {
            if ( auto* piece = this->pieces[i][j].get();
                piece
                && piece->getColor() == this->getTurn()) {
                if ( this->legalMoves(Coord{static_cast<char>(j + 'a'), i}).size() > 0 ) {
                    return false;
                }
            }
        }
    }
    return true;
}

const bool GameBoard::inCheck() {
    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {
            if ( auto* piece = this->pieces[i][j].get();
                piece
                && piece->getColor() != this->getTurn()) {
                std::vector<Coord> moves = legalMoves(Coord{static_cast<char>(j + 'a'), i}, true);
                for ( auto move : moves) {
                    if ( move == this->findKing(this->getTurn())) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


std::vector<Coord> GameBoard::checkCastle() {
    std::vector<Coord> moves;
    if ( this->inCheck() == true) {
        return moves;
    }
    if ( this->getTurn() == "white" ) {
        if ( whiteKingMoved == true ) return moves;
    }
    bool ok = false;
    int j = 0;
    if ( this->getTurn() == "black" ) {
        if ( blackKingMoved == true ) return moves;
        j = 7;
    }

    if ( auto* piece = pieces[j][0].get();
            piece
            && piece->type() == "Rook") {
        if ( auto* rook = dynamic_cast<Rook*>(piece)) {
            if ( rook->hasMoved() == false ) {
                ok = true;
            }
        }
            }
    for ( int i = 1; i < 4 && ok == true; i++ ) {
        if ( auto* piece = this->pieces[j][i].get();
            piece) {
            ok = false;
            }
        if ( i != 1 ) {
            if ( ok == true && !isSafeMove(Coord{'e', j}, Coord{static_cast<char>('a' + i), j})) {
                ok = false;
            }
        }
    }
    if ( ok == true ) {
        moves.push_back(Coord{'c', j});
    }

    ok = false;
    if ( auto* piece = pieces[j][7].get();
            piece
            && piece->type() == "Rook") {
        if ( auto* rook = dynamic_cast<Rook*>(piece)) {
            if ( rook->hasMoved() == false ) {
                ok = true;
            }
        }
            }
    for ( int i = 5; i < 7 && ok == true; i++ ) {
        if ( auto* piece = this->pieces[j][i].get();
            piece) {
            ok = false;
            }
        if ( ok == true && !isSafeMove(Coord{'e', j}, Coord{static_cast<char>('a' + i), j})) {
            ok = false;
        }
    }
    if ( ok == true ) {
        moves.push_back(Coord{'g', j});
    }


    ////// modifica if uri ca sa devina nested-if altfel poti verifica isSafeMode pe patratele unde exista piese
    return moves;
}



