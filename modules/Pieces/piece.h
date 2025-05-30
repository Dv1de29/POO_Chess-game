
#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <vector>
#include <filesystem>

#include "../ResourceManager/resource_manager.h"

#include "SFML/Graphics/Texture.hpp"

namespace fs = std::filesystem;

struct Coord{
    char col;
    int row;

    bool operator== (const Coord &other) const {
        return col == other.col && row == other.row;
    }
};

class Piece {
protected:
    ResourceManager<sf::Texture> textures_resources;
    std::shared_ptr<sf::Texture> texture;
public:
	Piece() {
        // try {
            for ( const auto& entry : fs::directory_iterator("../assets/Piece_Texture")) {
                if ( entry.is_regular_file() ) {
                    texture = textures_resources.load(entry.path().stem().string(), entry.path().string());
                    std::cerr << entry.path().stem().string() << std::endl;
                }
            }
        // } catch ( const std::exception &e ) {
            // throw std::runtime_error(e.what());
        // }
	}
    virtual ~Piece() = default;
    virtual void move() = 0;
    virtual std::vector<Coord> getMoves(Coord from) = 0;
    virtual std::string getColor() const = 0;
    virtual std::string type() const = 0;
    sf::Texture& getTexture() const { return *texture; };

};

class King : public Piece {
    std::string color;
public:
    explicit King(std::string color_) : color(std::move(color_)) {
        this->texture = textures_resources.get(color + "_king");
    }

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
        this->texture = textures_resources.get(color + "_knight");
    }
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
      this->texture = textures_resources.get(color + "_bishop");
  }
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
        this->texture = textures_resources.get(color + "_rook");
    }
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
        this->texture = textures_resources.get(color + "_queen");
    }
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
        this->texture = textures_resources.get(color + "_pawn");
    }
    ~Pawn() override = default;
    void move() override{};
    std::vector<Coord> getMoves(Coord from) override;

    std::string getColor() const override { return this->color; }
    std::string type() const override;
};



#endif //PIECE_H
