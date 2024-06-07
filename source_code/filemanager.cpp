#include "FileManager.h"
#include "Game.h"
#include "player.h"
#include <fstream>
#include <vector>
#include <list>
#include <utility> // For std::pair
#include <codecvt> // For std::wstring_convert
#include <locale> // For std::codecvt_utf8
#include <filesystem> 
#include <iostream>




FileManager::FileManager(const std::string& path) : filePath(path) {}


void FileManager::saveToFile(Game& game) {
    std::ofstream outFile("saving/saving.txt", std::ofstream::app);
    if (!outFile) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return;
    }

    // Save basic game state information
    outFile << "start" << std::endl;
    outFile << game.getScore() << std::endl;
    outFile << game.getMovesLeft() << std::endl;
    outFile << game.getplayername() << std::endl;
    outFile << game.getlevel()<< std::endl;

    // Serialize and save the grid state
    for (int y = 0; y < game.gridheight(); ++y) {
        for (int x = 0; x < game.gridwidth(); ++x) {
            outFile << game.getTileState(x, y) << " ";
        }
        outFile << std::endl; // New line for each row in the grid
    }
    outFile << "stop" << std::endl;


    outFile.close();
}

void FileManager::loadFromFile(std::string playerName, Game& game) {
    std::ifstream inFile("saving/saving.txt");
    if (!inFile) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }

    std::string line;
    bool isCurrentPlayer = false;
    Grid* grid = nullptr;

    while (std::getline(inFile, line)) {
        if (line == "start") {
            // Initialize variables to store game state information.
            int score, movesLeft;
            std::string currentName;
            std::string level;

            inFile >> score >> movesLeft >> currentName >> level;
            std::getline(inFile, line); // Consume the newline character after level

            if (currentName == playerName) {
                isCurrentPlayer = true;
                grid = new Grid(level);
                game.setMovesLeft(movesLeft);
                game.setplayername(currentName,score);
                game.setlevel(level);

                // Load grid state
                for (int y = 0; y < grid->getHeight(); ++y) {
                    for (int x = 0; x < grid->getWidth(); ++x) {
                        inFile >> line; // Read the tile state
                        if (!inFile) {
                            std::cerr << "Error reading tile state from file." << std::endl;
                            delete grid;
                            return;
                        }
                        grid->getTile(x, y).setState(line);
                    }
                }
                game.setGrid(grid); // Set the loaded grid to the game
            }
        }
        else if (line == "stop" && isCurrentPlayer) {
            break; // Successfully loaded the player's data
        }
    }

    if (!isCurrentPlayer) {
        std::cerr << "Player " << playerName << " not found in the save file." << std::endl;
        delete grid; // Cleanup if the player's name was not found after allocation
    }

    inFile.close();
}









std::vector<std::pair<std::wstring, int>> FileManager::getAllPlayers() {
    std::vector<std::pair<std::wstring, int>> players;
    std::wstring filePath = L"saving/saving.txt";
    std::wifstream inFile(filePath);


    if (!inFile) {
        std::wcerr << L"Failed to open file for reading: " << filePath << std::endl;
        return players;
    }

    std::wstring line;
    bool isReadingPlayer = false;
    std::wstring playerName;
    int score;

    while (std::getline(inFile, line)) {
        if (line == L"start") {
            inFile >> score;
            isReadingPlayer = true;
        }
        else if (line == L"stop") {
            if (isReadingPlayer && !playerName.empty()) {
                isReadingPlayer = false;
            }
        }
        else if (isReadingPlayer) {
            std::getline(inFile, line); // consume the remaining part of the line after the score
            std::getline(inFile, playerName); 
            players.push_back(std::make_pair(playerName, score));

            isReadingPlayer = false;


        }
    }

    inFile.close();
    return players;
}

std::vector<std::string> FileManager::getSavedPlayerNames() {
	std::vector<std::string> playerNames;
	std::ifstream inFile("saving/saving.txt");
    if (!inFile) {
		std::cerr << "Failed to open file for reading." << std::endl;
		return playerNames;
	}

	std::string line;
    while (std::getline(inFile, line)) {
        if (line == "start") {
			int score, movesLeft;
			std::string playerName;
			std::string level;
			inFile >> score >> movesLeft >> playerName >> level;
			playerNames.push_back(playerName);
			// Skip lines until the grid state starts
            for (int y = 0; y < 10; ++y) {
				std::getline(inFile, line); // Consume the grid state lines
			}
		}
	}

	inFile.close();
	return playerNames;
}
