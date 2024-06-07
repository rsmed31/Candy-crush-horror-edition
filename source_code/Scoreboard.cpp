#include "Scoreboard.h"
#include "FileManager.h"
#include <algorithm> // For std::sort
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <locale>
#include <codecvt>
#include <list>
#include "game.h"

bool comparePlayers(const std::pair<std::wstring, int>& a, const std::pair<std::wstring, int>& b) {
    return a.second > b.second; 
}

std::list<std::string> Scoreboard::displayScores(sf::RenderWindow& window, FileManager filemanager) {
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return std::list<std::string>(); 
    }

    std::vector<std::pair<std::wstring, int>> players = filemanager.getAllPlayers();

    // Sort players by score in descending order before displaying
    std::sort(players.begin(), players.end(), comparePlayers);

    float yPos = 300.0f; // Starting y position for the first score
    std::list<std::string> li;
    // Converter for wide to narrow string conversion
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    window.clear();

    sf::Text returnText;
    // Setup high scores title text
    returnText.setFont(font);
    returnText.setString("Highscores:");
    returnText.setCharacterSize(35);
    returnText.setFillColor(sf::Color::Red);
    returnText.setPosition(50.0f, 260.0f);
    window.draw(returnText); // Draw title text

    for (const auto& player : players) {
        sf::Text text;
        text.setFont(font);

        // Convert the player's name from wstring to string
        std::string playerName = converter.to_bytes(player.first);
        text.setString(playerName + ": " + std::to_string(player.second));

        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(50.0f, yPos);

        window.draw(text); // Draw player name and score
        yPos += 30.0f; // Increment y position for next score
        li.push_back(playerName);
    }

    return li;
}
