#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Grid.h"
#include "Scoreboard.h"
#include "game.h"
#include <SFML/Audio.hpp>

// Constructor: Initializes the menu with the game window and sets up menu items.
Menu::Menu(sf::RenderWindow& win) : window(win), selectedItemIndex(0) {
    // Load a font
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Menu options
    std::string options[] = { "Play Game", "Load Game", "Scoreboard", "Exit" };
    sf::Vector2f startPosition(100.f, 100.f); // Starting position for the first menu item
    float verticalSpacing = 300.f; // Spacing between menu items

    for (auto& option : options) {
        sf::Text text;
        text.setFont(font);
        text.setString(option);
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::White);
        text.setPosition(startPosition);

        menuItems.push_back(text);
        startPosition.y += verticalSpacing; // Move down for the next item
    }

    // Highlight the first menu item
    menuItems[0].setFillColor(sf::Color::Red);
}

// Draws the menu items to the window
void Menu::draw() {

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    for (auto& item : menuItems) {

        if (item.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
            item.setFillColor(sf::Color::Red);
        }
       

        window.draw(item);
    }


}


// Navigates through the menu items based on user input
void Menu::navigate(int move) {

    menuItems[selectedItemIndex].setFillColor(sf::Color::White);

    selectedItemIndex += move;
    if (selectedItemIndex < 0) selectedItemIndex = menuItems.size() - 1;
    else if (selectedItemIndex >= menuItems.size()) selectedItemIndex = 0;

    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

}



// Handles user keyboard input for menu navigation and selection
bool Menu::handleInput(const sf::Event& event) {
    // Keyboard navigation
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            navigate(-1); // Move selection up
        }
        else if (event.key.code == sf::Keyboard::Down) {
            navigate(1); // Move selection down
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            // This part must work for both the main menu and the difficulty menu
            std::string selectedItemLabel = menuItems[selectedItemIndex].getString();
            if (actions.find(selectedItemLabel) != actions.end()) {
                actions[selectedItemLabel](); // Execute the action
            }
            return true; // Return true if an action was executed
        }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            for (size_t i = 0; i < menuItems.size(); ++i) {
                if (menuItems[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    selectedItemIndex = i; // Update selected item based on click
                    auto actionIter = actions.find(menuItems[i].getString().toAnsiString());
                    if (actionIter != actions.end()) {
                        actionIter->second(); // Execute the action
                    }

                    return true; // Return true if an action was executed

                    break;
                }
            }
        }
    }

    return false; // Return false if no action was executed


}

void Menu::addItem(const std::string& label, std::function<void()> action) {
    sf::Text text;
    text.setFont(font); // Assume font is loaded
    text.setString(label);
    text.setCharacterSize(50); // Example size
    // Correct positioning calculation
    text.setPosition(100.f, 150.f + menuItems.size() * 70.f);

    menuItems.push_back(text);
    actions[label] = action; // Map label to action
}

void Menu::clearItems() {
    menuItems.clear(); // Clear menu items
    actions.clear(); // Clear actions if necessary
}





