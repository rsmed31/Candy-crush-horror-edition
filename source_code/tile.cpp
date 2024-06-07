#include "Tile.h"
#include "Grid.h"
#include "Scoreboard.h"
#include "Game.h"
#include "Menu.h"
#include "FileManager.h"
#include <list>
#include <string>
#include <SFML/Graphics.hpp>
#include "Tile.h"

// Draw the tile to the render target
void Tile::draw(sf::RenderTarget& target, sf::Vector2f position) const {
    if (this->isEmpty() == false) {
        sf::Sprite adjustedSprite = sprite; // Copy to adjust position without altering original sprite
        adjustedSprite.setPosition(position);
        target.draw(adjustedSprite);
    }



}
sf::Vector2f Tile:: getPosition() const {
    return sprite.getPosition();
}
void Tile::setPosition(sf::Vector2f position) {
    sprite.setPosition(position);
}

void Tile::setCandyType(CandyType type) {
    candyType = type;
}

CandyType Tile::getCandyType() const {
    return candyType;
}
// Load a texture from a file, set the sprite, and mark the tile as textured
bool Tile::loadTexture(const std::string& filename) {
    if (!texture.loadFromFile(filename)) {
        return false; // Failed to load the texture
    }
    sprite.setTexture(texture);
    return true; // Successfully loaded the texture
}

void Tile::setColor(const sf::Color& newColor) {
    color = newColor;
}

sf::Color Tile::getColor() const {
    return color;
}
sf::Texture Tile::gettexture() const {
    return texture;
}



bool Tile::isEmpty() const {
    return this->empty;
}

void Tile::clear() { // Call this method to mark a tile as empty after a match
    this->empty = true;
}
void Tile::unClear() { // Call this method to mark a tile as empty after a match
    this->empty = false;
}
std::string Tile::getState() const {
    switch (getCandyType()) {
    case CandyType::GREEN: return "GREEN";
    case CandyType::PURPLE: return "PURPLE";
    case CandyType::ORANGE: return "ORANGE";
    case CandyType::YELLOW: return "YELLOW";
    case CandyType::RED: return "RED";
    case CandyType::BLUE: return "BLUE";
    case CandyType::EXPLOSIVE: return "EXPLOSIVE";
    default: return "NONE";
    }
}

void Tile::setState(const std::string& state) {
    if (state == "GREEN") {
        setCandyType(CandyType::GREEN);
        Tile::loadTexture("images/objects/nuclear.png");
    }
    else if (state == "PURPLE") {
        setCandyType(CandyType::PURPLE);
        Tile::loadTexture("images/objects/spider.png");
    }
    else if (state == "ORANGE") {
        setCandyType(CandyType::ORANGE);
        Tile::loadTexture("images/objects/skull.png");
    }
    else if (state == "YELLOW") {
        setCandyType(CandyType::YELLOW);
        Tile::loadTexture("images/objects/ghost.png");
    }
    else if (state == "RED") {
        setCandyType(CandyType::RED);
        Tile::loadTexture("images/objects/candles.png");
    }
    else if (state == "BLUE") {
        setCandyType(CandyType::BLUE);
        Tile::loadTexture("images/objects/bone.png");
    }
    else if (state == "EXPLOSIVE") {
        setCandyType(CandyType::EXPLOSIVE);
        Tile::loadTexture("images/objects/explosive.png");
    }
    else {
        setCandyType(CandyType::NONE);
    }
}



