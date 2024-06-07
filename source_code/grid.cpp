#include "Grid.h"
#include "Game.h"
#include "Tile.h"
#include <vector>
#include <SFML/Graphics.hpp>

std::vector<std::string> candyFiles = {
    "images/objects/nuclear.png",
    "images/objects/spider.png",
    "images/objects/skull.png",
    "images/objects/ghost.png",
    "images/objects/candles.png",
    "images/objects/bone.png",
    "images/objects/explosive.png"
};


Grid::Grid(const std::string& diff) : difficulty(diff) {
    if (difficulty == "Easy") {
        width = 10;
        height = 10;       
    }
    else if (difficulty == "Medium") {
        width = 8;
        height = 8;
    }
    else if (difficulty == "Hard") {

        width = 5;
        height = 5;
    }
    else {
        // Default or error handling
        width = 10;
        height = 10;
    }
    tiles.resize(height, std::vector<Tile>(width)); // Resize and initialize tiles
    preventImmediateMatches(); // Custom method to adjust candy selection to prevent matches
        
    
}





void Grid::preventImmediateMatches() {
    // Initially, randomly assign candy types to all tiles
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int candyIndex = std::rand() % 6;
            tiles[y][x].setCandyType(static_cast<CandyType>(candyIndex));
            tiles[y][x].loadTexture(candyFiles[candyIndex]);
            tiles[y][x].unClear(); // Mark the tile as filled
        }
    }

    // Iterate over the grid to replace candies that form matches
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {;
            while (checkForPotentialMatch(x, y)) {
                int candyIndex = std::rand() % 6;
                tiles[y][x].setCandyType(static_cast<CandyType>(candyIndex));
                tiles[y][x].loadTexture(candyFiles[candyIndex]);
                tiles[y][x].unClear(); // Mark the tile as filled
            }
        }
    }

}













bool Grid::isValidMove(int x, int y, int dx, int dy) const {
    // Ensure dx and dy represent a move to an adjacent tile
    if ((abs(dx) == 1 && dy == 0) || (abs(dy) == 1 && dx == 0)) {

        return true;
    }
    return false; // Not a valid move if not to an adjacent tile
}



int Grid::swapTiles(int x, int y, int dx, int dy) {
    if (!isValidMove(x, y, dx, dy)) return 0;

    // Calculate new positions
    int newX = x + dx, newY = y + dy;
    int newscore = 0;
    int totalScore = 0;
    sf::Vector2f pos1 = tiles[y][x].getPosition();
    sf::Vector2f pos2 = tiles[newY][newX].getPosition();



    // Perform the swap
    std::swap(tiles[y][x], tiles[newY][newX]);
    // Check if the swap resulted in any matches
    newscore = findAndRemoveMatches();
    if (newscore > 0) {

        totalScore += newscore;
        dropTiles();
        refill();
        newscore = findAndRemoveMatches();
        while (newscore > 0) {
            totalScore += newscore;
            dropTiles();
            refill();
            newscore = findAndRemoveMatches();

        }
        if (totalScore >= 4) {
            tiles[y][x].setCandyType(CandyType::EXPLOSIVE);
			tiles[y][x].loadTexture("images/objects/explosive.png");
            tiles[y][x].unClear();
		}
        return totalScore;
    }
    else {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                tiles[y][x].unClear(); // Mark all tiles as filled
            }
        }


      

        std::swap(tiles[y][x], tiles[newY][newX]);
        


        return 0;
    }
}


int Grid::findAndRemoveMatches() {
    std::vector<std::vector<bool>> markedForRemoval(height, std::vector<bool>(width, false));
    int matchFound = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (checkForPotentialMatch(x, y)) { // If a match is found at the current position
                markedForRemoval[y][x] = true; // Mark the tile for removal
            }
        }
    }
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (markedForRemoval[y][x]) {

                tiles[y][x].clear(); // Mark the tile as empty
                matchFound++;

            }
            if (tiles[y][x].getState() == "EXPLOSIVE") {
                for (int i = 0; i < width; ++i) {
                    if (tiles[y][i].isEmpty() == false) {
						tiles[y][i].clear();
						matchFound++;
					}
				}
                for (int i = 0; i < height; ++i) {
                    if (tiles[i][x].isEmpty() == false) {
						tiles[i][x].clear();
						matchFound++;
					}
				}
			}
        }
    }
    return matchFound;
}




bool Grid::checkForPotentialMatch(int x, int y) {
    CandyType currentType = tiles[y][x].getCandyType();
    if(tiles[y][x].isEmpty()) return false; // No match if the tile is empty
    int matchCount = 1;

    // Check horizontally (left and right)
    if (x - 1 >= 0) {
        if (tiles[y][x - 1].getCandyType() == currentType && tiles[y][x - 1].isEmpty() == false) {
            matchCount++;
            if (x - 2 >= 0) {
                if (tiles[y][x - 2].getCandyType() == currentType && tiles[y][x - 2].isEmpty() == false) matchCount++;
            }
        }
    }
    if (x + 1 < width) {
        if (tiles[y][x + 1].getCandyType() == currentType && tiles[y][x + 1].isEmpty() == false) {
			matchCount++;
            if (x + 2 < width) {
                if (tiles[y][x + 2].getCandyType() == currentType && tiles[y][x+2].isEmpty() == false) matchCount++;
            }
		}
	}
    if (matchCount >= 3) return true; // Found a horizontal match


    matchCount = 1;
    if (y - 1 >= 0) {
        if (tiles[y - 1][x].getCandyType() == currentType && tiles[y - 1][x].isEmpty() == false) {
			matchCount++;
            if (y - 2 >= 0) {
				if (tiles[y - 2][x].getCandyType() == currentType && tiles[y - 2][x].isEmpty() == false) matchCount++;
			}
		}
	}
    if (y + 1 < height) {
        if (tiles[y + 1][x].getCandyType() == currentType && tiles[y+1][x].isEmpty()==false) {
            matchCount++;
            if (y + 2 < height) {
				if (tiles[y + 2][x].getCandyType() == currentType && tiles[y + 2][x].isEmpty() == false) matchCount++;
			}
        }
    }

    if (matchCount >= 3) return true; // Found a vertical match

    return false; // No match found
}




// Drop tiles to fill empty spaces
void Grid::dropTiles() {
    Animation animation;
    for (int x = 0; x < width; ++x) {
        for (int y = height - 2; y >= 0; --y) { // Start above the bottom row
            if (!tiles[y][x].isEmpty() && tiles[y + 1][x].isEmpty()) { // If there's a tile to drop
                int dropRow = y;
                while (dropRow + 1 < height && tiles[dropRow + 1][x].isEmpty()) { // Find the lowest empty position
                    std::swap(tiles[dropRow][x], tiles[dropRow + 1][x]);
                    dropRow++;



                }
            }
        }
    }
}




// Refill the grid with new candies
void Grid::refill() {
    Animation animation;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (tiles[y][x].isEmpty()) { // Only refill empty tiles
                int candyIndex = std::rand() % candyFiles.size();
                tiles[y][x].setCandyType(static_cast<CandyType>(candyIndex));
                while ("EXPLOSIVE" == tiles[y][x].getState()) {
                    candyIndex = std::rand() % candyFiles.size();
                    tiles[y][x].setCandyType(static_cast<CandyType>(candyIndex));
                }
                tiles[y][x].unClear(); // Mark the tile as filled

            }
        }
    }
}



void Grid::draw(sf::RenderWindow& target, sf::Vector2u windowSize) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(target);
    float tileSize = 50.0f;
    float gridWidth = width * tileSize;
    float gridHeight = height * tileSize;
    sf::Vector2f startPosition( (windowSize.x- gridWidth) / 2, (windowSize.y - gridHeight) / 2);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            sf::Vector2f position = startPosition + sf::Vector2f(x * tileSize, y * tileSize);
            // Check if mouse is over this tile
            if (tiles[y][x].isEmpty() == false) {
                tiles[y][x].loadTexture(candyFiles[static_cast<int>(tiles[y][x].getCandyType())]);
                tiles[y][x].draw(target, position);
            }
            if (mousePos.x > position.x && mousePos.x < position.x + tileSize &&
                mousePos.y > position.y && mousePos.y < position.y + tileSize) {
                sf::RectangleShape highlight(sf::Vector2f(tileSize, tileSize));
                highlight.setPosition(position);
                highlight.setFillColor(sf::Color(255, 255, 255, 128)); // Semi-transparent white
                target.draw(highlight);

            }

        }
    }


}






sf::Vector2f Grid::getOffset(sf::Vector2u windowsize) const {
    return sf::Vector2f((windowsize.x - width * 50) / 2, (windowsize.y - height * 50) / 2);
}

int Grid::getTileSize() const {
    return 50; // Fixed tile size as used in Grid::draw
}



bool Grid::GridOver() {
        // Check every tile in the grid
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Check the possibility of a horizontal swap
                if (x < width - 1) {
                    // Simulate a swap with the right tile
                    if (isSwapPossible(x, y, x + 1, y)) {
                        return false; // Found a possible move
                    }
                }
                // Check the possibility of a vertical swap
                if (y < height - 1) {
                    // Simulate a swap with the bottom tile
                    if (isSwapPossible(x, y, x, y + 1)) {
                        return false; // Found a possible move
                    }
                }
            }
        }
        return true; // No possible moves left
    }

bool Grid::isSwapPossible(int x1, int y1, int x2, int y2){
    // Temporarily swap candies
    CandyType temp = tiles[y1][x1].getCandyType();
    tiles[y1][x1].setCandyType(tiles[y2][x2].getCandyType());
    tiles[y2][x2].setCandyType(temp);

    bool matchFound = false;

    // Check for matches at the new positions
    if (checkForPotentialMatch(x1, y1) || checkForPotentialMatch(x2, y2)) {
        matchFound = true;
    }

    // Revert the swap
    tiles[y2][x2].setCandyType(tiles[y1][x1].getCandyType());
    tiles[y1][x1].setCandyType(temp);

    return matchFound;
}