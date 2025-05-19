
#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <vector>

#include "SFML/Graphics/Texture.hpp"

struct Coord{
    char col;
    int row;

    bool operator== (const Coord &other) const {
        return col == other.col && row == other.row;
    }
};

class Piece {
protected:
    sf::Texture texture;
public:
	Piece() = default;
    virtual ~Piece() = default;
    virtual void move() = 0;
    virtual std::vector<Coord> getMoves(Coord from) = 0;
    virtual std::string getColor() const = 0;
    virtual std::string type() const = 0;
    sf::Texture& getTexture() { return texture; };

};

class King : public Piece {
    std::string color;
public:
    explicit King(std::string color_) : color(std::move(color_)) {
        if ( !this->texture.loadFromFile("../assets/" + color + "_king.png") ) {
            throw std::runtime_error("Failed to load King texture");
        }
    };
    ~King() override = default;
    void move() override{};
    std::vector<Coord> getMoves(Coord from) override;

    std::string getColor() const override { return this->color; }
    std::string type() const override;
};
class Knight : public Piece {
    std::string color;
public:
    explicit Knight(std::string color_) : color(std::move(color_)) {
        if ( !this->texture.loadFromFile("../assets/" + color + "_knight.png") ) {
            throw std::runtime_error("Failed to load Knight texture");
        }
    };
    ~Knight() override = default;
    void move() override{};
    std::vector<Coord> getMoves(Coord from) override;

    std::string getColor() const override { return this->color; }
    std::string type() const override;
};
class Bishop : public Piece {
    std::string color;
public:
  explicit Bishop(std::string color_) : color(std::move(color_)) {
      if ( !this->texture.loadFromFile("../assets/" + color + "_bishop.png") ) {
          throw std::runtime_error("Failed to load Bishop texture");
      }
  };
  ~Bishop() override = default;
  void move() override{};
    std::vector<Coord> getMoves(Coord from) override;

    std::string getColor() const override { return this->color; }
    std::string type() const override;
};
class Rook : public Piece {
    std::string color;
    bool Moved = false;
  public:
    explicit Rook(std::string color_) : color(std::move(color_)) {
        if ( !this->texture.loadFromFile("../assets/" + color + "_rook.png") ) {
            throw std::runtime_error("Failed to load Rook texture");
        }
    };
    ~Rook() override = default;
    void move() override{};
    std::vector<Coord> getMoves(Coord from) override;

    std::string getColor() const override { return this->color; }
    std::string type() const override;

    void setMoved();
    const bool hasMoved();
};
class Queen : public Piece {
    std::string color;
  public:
    explicit Queen(std::string color_) : color(std::move(color_)) {
        if ( !this->texture.loadFromFile("../assets/" + color + "_queen.png") ) {
            throw std::runtime_error("Failed to load Queen texture");
        }
    };
    ~Queen() override = default;
    void move() override{};
    std::vector<Coord> getMoves(Coord from) override;

    std::string getColor() const override { return this->color; }
    std::string type() const override;
};
class Pawn : public Piece {
    std::string color;
public:
    explicit Pawn(std::string color_) : color(std::move(color_)) {
        std::cout << color << '\n' << "DE CE NU MERGE VEVERITAAAAAA\n";
        if ( !this->texture.loadFromFile("../assets/" + color + "_pawn.png") ) {
            throw std::runtime_error("Failed to load Pawn texture");
        }
    };
    ~Pawn() override = default;
    void move() override{};
    std::vector<Coord> getMoves(Coord from) override;

    std::string getColor() const override { return this->color; }
    std::string type() const override;
};



#endif //PIECE_H
