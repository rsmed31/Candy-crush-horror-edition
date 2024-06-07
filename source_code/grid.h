#ifndef _GRID_H_
#define _GRID_H_

#include <iostream>
#include <vector>
#include "Tile.h"

class Grid {
    std::vector<std::vector<Tile>> tiles;
    int width;
    int height;
    std::string difficulty;


public:
    Grid(const std::string& difficulty);

    bool isValidMove(int x, int y, int dx, int dy) const;
    int swapTiles(int x, int y, int dx, int dy);
    int findAndRemoveMatches();
    void dropTiles();
    void refill();
    void setdifficulty(std::string difficulty) {
        this->difficulty = difficulty;
    }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Tile& getTile(int x, int y) { return tiles[y][x]; }
    void draw(sf::RenderWindow& target, sf::Vector2u windowSize);
    void preventImmediateMatches();

    bool isValidPosition(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    sf::Vector2f getOffset(sf::Vector2u windowsize) const;
    int getTileSize() const;

    int getgridwidth() { return width; };
    int getgridheight() { return height; };
    std::string getTileState(int x, int y) const {
        if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
            throw std::out_of_range("Grid::getTileState called with out-of-bounds coordinates.");
        }
        // Directly access the Tile at (x, y) and call its getState method
        return this->tiles[y][x].getState();
    }
    bool GridOver();
    bool isSwapPossible(int x1, int y1, int x2, int y2);
    bool checkForPotentialMatch(int x, int y);

    sf::Vector2f calculatePosition(int x, int y) {
        float offsetX = 100.0f; // Example offset from the left edge of the window
        float offsetY = 50.0f; // Example offset from the top edge of the window
        float tileSize = 32.0f; // Size of each tile

        return sf::Vector2f(offsetX + x * tileSize, offsetY + y * tileSize);
    }
    std::string getdifficulty() {
        return difficulty;
    }
};

#endif
