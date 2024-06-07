// Game.cpp
#include "Game.h"
#include "grid.h"
#include "menu.h"
#include "player.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include "animation.h"

#include <iostream>

sf::VideoMode desktop = sf::VideoMode::getDesktopMode();



Game::Game() : window(desktop, "Candy Crush Horror edition"), menu(window) {
    sf::Image icon;
    if (!icon.loadFromFile("images/logo/symbol.png")) {
        // Handle error
        std::cerr << "Could not load icon image" << std::endl;
    }

    // Set the icon for the window
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    score = 0;
    movesLeft = 30;
    fileManager = std::make_unique<FileManager>("savefile.txt");
    grid = std::make_unique<Grid>("Easy");
    player = std::make_unique<Player>("Initial Player Name");
    window.setVerticalSyncEnabled(true);
    gameState = GameState::MainMenu;


    setupMenuActions();
}


void Game::setupMenuActions() {
    // Clear any existing items if this can be called multiple times
    menu.clearItems();

    // Add menu items with their corresponding actions
    menu.addItem("Play Game", [this]() { gameState = GameState::SelectDifficulty; });
    menu.addItem("Load Game", [this]() { this->loadGame(); });
    menu.addItem("Scoreboard", [this]() { gameState = GameState::Scoreboard; });
    menu.addItem("Exit", [this]() { this->exitGame(); }); 

}




void Game::saveGame() {

    sf::Font font;
    font.loadFromFile("font.ttf");
    sf::Text text("", font, 50);
    text.setFillColor(sf::Color::White);
    text.setPosition(500, 402); // Adjust as needed 
    sf::Text save("SAVING YOUR PROGRESS", font, 50);
    save.setFillColor(sf::Color::Red);
    save.setPosition(500, 350); // Adjust as needed

    // Create a visual "input box" around the text area
    sf::RectangleShape inputBox(sf::Vector2f(400, 60)); // Width and height of the box
    inputBox.setPosition(498, 400); // Slightly behind text for visual effect
    inputBox.setFillColor(sf::Color::Transparent);
    inputBox.setOutlineColor(sf::Color::White);
    inputBox.setOutlineThickness(5);


    sf::Text placeholderText("Enter your name", font, 50);
    placeholderText.setFillColor(sf::Color::White);
    placeholderText.setPosition(500, 402); // Position above the input box

    std::string playerName;
    window.draw(inputBox); // Draw the input box



    while (window.isOpen() && gameState == GameState::Saving) {
        sf::Event event;
        if (playerName.empty()) {
            window.clear(); // Clear the window to redraw
            window.draw(placeholderText); // Draw placeholder text if no input yet
            window.draw(inputBox); // Redraw the input box for visual feedback

            window.display(); // Update the display to show changes
        }
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::TextEntered) {

               
                if (event.text.unicode == '\b' && !playerName.empty()) { // handle backspace
                    playerName.pop_back();
                }
                else if (event.text.unicode < 128 && event.text.unicode != '\b') { // handle normal characters
                    playerName += static_cast<char>(event.text.unicode);
                }
                text.setString(playerName); // Update text with current input
            }

            window.clear(); // Clear the window to redraw
            window.draw(inputBox); // Redraw the input box for visual feedback
            window.draw(text); // Draw the updated text
            window.display(); // Update the display to show changes

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    // Fetch existing player names
                    std::vector<std::string> existingNames = fileManager->getSavedPlayerNames();

                    // Check if entered name exists in the list
                    if (std::find(existingNames.begin(), existingNames.end(), playerName) != existingNames.end()) {
                        // Name exists, handle accordingly
                        sf::Text errorText("Name already exists! Enter a different name.", font, 20);
                        errorText.setFillColor(sf::Color::Red);
                        errorText.setPosition(500, 500); // Adjust as needed
                        while (window.pollEvent(event)) {} // Clear event queue
                        sf::Clock errorClock;
                        while (errorClock.getElapsedTime().asSeconds() < 2) {
                            window.clear();
                            window.draw(inputBox);
                            window.draw(text);
                            window.draw(errorText);
                            window.display();
                        }
                        continue; // Skip saving and allow the user to enter a different name
                    }
                setplayername(playerName, score);
                fileManager->saveToFile(*this);
                playerName = ""; // Reset player name for next input
                gameState = GameState::loading;

                sf::Text suc("Succeffuly saved", font, 100);
                suc.setFillColor(sf::Color::Red);
                suc.setPosition(500, 402); // Position above the input box
                sf::Clock clock; // Commence à mesurer le temps ici
                while (clock.getElapsedTime().asSeconds() < 2) {
                    window.clear();
                    window.draw(suc);
                    window.display();
                }
                this->startGame();


                break;
            }
        }
    }
}


void Game::setplayername(std::string name, int score_) {
	this->player->setname(name);
    this->player->setscore(score);
    score = score_;
}

void Game::exitGame() {
    window.close();
}

void Game::run() {
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;

    animation.PlayMusic("sounds/background.ogg");
    
    while (window.isOpen()) {
        this->movesLeft = 1;
        this->score = 0;
        processEvents(window.getSize()); // Process input events
        backgroundTexture.loadFromFile("images/Background/frame1.png");
        backgroundSprite.setTexture(backgroundTexture);
        animation.MainMenuAnim(&backgroundSprite, &backgroundTexture, "Background",4);
        // Calculate the scale factors
        float scaleX = static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y;

        // Set the scale of the sprite
        backgroundSprite.setScale(scaleX, scaleY);


        window.clear(); // Clear the screen
        window.draw(backgroundSprite);

        if (gameState == GameState::MainMenu) {
            setupMenuActions();
            menu.draw(); // Draw the menu in either state
            window.setVerticalSyncEnabled(true);
            window.display(); // Display the rendered frame
        }

        if (gameState == GameState::SelectDifficulty) {
            showDifficultyMenu();
            menu.draw(); // Draw the menu in either state
            window.setVerticalSyncEnabled(true);
            window.display(); // Display the rendered frame
        }
        else if (gameState == GameState::Playing) {
            (*this).startGame(); // Draw the game components
            window.display(); // Display the rendered frame
        }
        else if (gameState == GameState::loading) {
            (*this).startGame(); // Draw the game components

            window.display(); // Display the rendered frame

        }



       
        else if (gameState == GameState::Paused) {
            Pause();
			menu.draw(); // Draw the menu in either state
			window.setVerticalSyncEnabled(true);
            window.display(); // Display the rendered frame

		}   
        if (gameState == GameState::Scoreboard) {
            window.setVerticalSyncEnabled(true);
            scoreboard.displayScores(window, *fileManager);
            menu.clearItems(); // Clears the main menu items
            menu.addItem("Return", [this]() {this->setupMenuActions(); });
            menu.draw();
            window.display(); // Display the rendered frame
            
        }

    }
}


// In Game.cpp
void Game::processEvents(sf::Vector2u windowsize) {
    sf::Event event;
    static bool isDragging = false;
    static int dragStartX = -1, dragStartY = -1; // Track the drag start position

    sf::Font font;
    font.loadFromFile("font.ttf");

    sf::RectangleShape pauseButton;
    sf::Text pauseText;

    // Define margins for the button
    const float marginX = 10.0f; // Horizontal margin
    const float marginY = 10.0f; // Vertical margin

    // Setup pause button with margins
    pauseButton.setSize(sf::Vector2f(100, 50));
    pauseButton.setPosition(marginX, marginY); // Use margins here
    pauseButton.setFillColor(sf::Color::White);

    // Setup pause text
    pauseText.setFont(font);
    pauseText.setString("Pause");
    pauseText.setCharacterSize(35);
    pauseText.setFillColor(sf::Color::Black);

    // Center the text on the button
    sf::FloatRect textRect = pauseText.getLocalBounds();
    pauseText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    pauseText.setPosition(sf::Vector2f(pauseButton.getPosition().x + pauseButton.getSize().x / 2.0f,
        pauseButton.getPosition().y + pauseButton.getSize().y / 2.0f));


    // In your render loop
    window.draw(pauseButton);
    window.draw(pauseText); // Draw text after the button so it appears on top


    sf::Text scoreText;
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setFont(font);
    scoreText.setCharacterSize(70); // in pixels
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition((window.getSize().x - scoreText.getLocalBounds().width) / 2, 10);
    window.draw(scoreText);

    sf::Text moveleft;
    moveleft.setString("Movesleft: " + std::to_string(movesLeft));
    moveleft.setFont(font);
    moveleft.setCharacterSize(70); // in pixels
    moveleft.setFillColor(sf::Color::Red);
    moveleft.setPosition((window.getSize().x - moveleft.getLocalBounds().width) / 2, window.getSize().y - moveleft.getLocalBounds().height - 100);
    window.draw(moveleft);



    if (gameState == GameState::Saving) {
        saveGame();
    }
    if (gameState == GameState::Scoreboard) {
        scoreboard.displayScores(window, *fileManager);
        menu.clearItems(); // Clears the main menu items
        menu.addItem("Return", [this]() {gameState = GameState::MainMenu; });
        menu.draw();
    }
   

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (gameState == GameState::Playing || gameState == GameState::loading) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                int gridX = (mouseX - grid->getOffset(windowsize).x) / grid->getTileSize();
                int gridY = (mouseY - grid->getOffset(windowsize).y) / grid->getTileSize();
                if (grid->isValidPosition(gridX, gridY)) {
                    isDragging = true;
                    dragStartX = gridX;
                    dragStartY = gridY;
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (isDragging) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;
                    int gridX = (mouseX - grid->getOffset(windowsize).x) / grid->getTileSize();
                    int gridY = (mouseY - grid->getOffset(windowsize).y) / grid->getTileSize();

                    if (grid->isValidPosition(gridX, gridY)) {
                        if (grid->isValidMove(dragStartX, dragStartY, gridX - dragStartX, gridY - dragStartY)) {
                            int newScore = grid->swapTiles(dragStartX, dragStartY, gridX - dragStartX, gridY - dragStartY);
                            if (newScore > 0) {
                                this->update(newScore);
                                animation.StopMusic();
                                animation.PlayMusic("sounds/click.ogg");
                                animation.showImageAt(window, mouseX, mouseY, "images/effects/blood.png");


                                grid->draw(window, window.getSize());



                            }
                            else {
                                this->update(0);
                                animation.StopMusic();
                                animation.PlayMusic("sounds/error.ogg");
                                animation.showImageAt(window, mouseX, mouseY, "images/effects/error.png");
                            }

                        }
                        else {
                            this->update(0);
                            animation.StopMusic();
                            animation.PlayMusic("sounds/error.ogg");
                            animation.showImageAt(window, mouseX, mouseY, "images/effects/error.png");


                        }
                    }
                    isDragging = false;
                    dragStartX = -1;
                    dragStartY = -1;
                }
            }
        }
        else {
            if (menu.handleInput(event)) {
                animation.StopMusic();
                animation.PlayMusic("sounds/click2.ogg");
            }
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (pauseButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    gameState = gameState == GameState::Playing ? GameState::Paused : GameState::Playing;
                }
            }
        }

        if (event.type == sf::Event::Resized) {
            // Adjust the viewport to the new window size
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));

            float newGridPosX = event.size.width / 2.0f - (grid->getgridwidth() / 2.0f);
            float newGridPosY = event.size.height / 2.0f - (grid->getgridheight() / 2.0f);
            grid->draw(window, window.getSize());
        }
    }
}






void Game::update(int newscore) {
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return; // Ensure the font is loaded successfully
    }
    std::vector<std::string>   DText;
    if (newscore > 0) {
        this->score = this->score + newscore;
        this->movesLeft = this->movesLeft - 1;

        DText=  {
            "well done!",
            "good job!",
            "nice move!",
            "keep it up!",
            "great job!",
        };

    }
    else {
    DText= {
    "watch out",
    "im coming for you",
    "its over soon",
    "you are a dead person",
    "i will kill you",
        };
        
    }
    int Index = std::rand() % DText.size();

    sf::Vector2f position(1300, 525);
    sf::Text Text;
    Text.setString(DText[Index]);
    Text.setFont(font);
    Text.setCharacterSize(75);
    Text.setFillColor(sf::Color::Red); // Color for emphasis
    Text.setPosition(position);
    sf::Clock clock; // Commence à mesurer le temps ici
    while (window.isOpen() && clock.getElapsedTime().asSeconds() < 0.5) {
        window.clear();
        grid->draw(window, window.getSize());
        processEvents(window.getSize());
        window.draw(Text);
        window.display();
    }


}













void Game::startGame() {
    if (gameState == GameState::Playing) {
        grid = std::make_unique<Grid>(getlevel());
    }
    while (window.isOpen()) {
        // Event handling...
        if (gameState == GameState::MainMenu) {
            break;
		}
        window.clear();
        grid->draw(window, window.getSize());
        sf::Vector2u windowsize=window.getSize();
        // Draw other game elements like UI...
        (*this).endGame();

        processEvents(windowsize); // Process input events

        if (gameState == GameState::Paused) {
            Pause();
            menu.draw();
        }
        if (gameState == GameState::GameOver) {
			endGame();
		}
        if (gameState == GameState::Saving) {
			saveGame();

		}

        window.display();
    }
}



void Game::endGame() {
    int current = getScore();
    std::string currentscore = std::to_string(current);
    if (movesLeft <=0 || grid->GridOver()) {
        sf::Font font;
        sf::Text reason;
        reason.setFont(font);
        reason.setCharacterSize(30);

        font.loadFromFile("font.ttf");
        if (movesLeft <= 0) {
            reason.setString("no moves left");

		}
        if (grid->GridOver()) {
			reason.setString("no possible moves you can make");
		}
		gameState = GameState::MainMenu;
        setMovesLeft(0);
        sf::Clock clock; 
        sf::Event event;
        window.pollEvent(event);
        while (clock.getElapsedTime().asSeconds() < 5 && gameState != GameState::Playing) {

		window.clear();
        menu.clearItems();
        if (gameState == GameState::Playing) {
            break;
		}
        sf::Sprite backgroundSprite;
        sf::Texture backgroundTexture;
        backgroundTexture.loadFromFile("images/gameover/frame1.png");
        backgroundSprite.setTexture(backgroundTexture);
        animation.MainMenuAnim(&backgroundSprite, &backgroundTexture, "gameover",4);
        // Calculate the scale factors
        float scaleX = static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y;

        // Set the scale of the sprite
        backgroundSprite.setScale(scaleX, scaleY);
        window.draw(backgroundSprite);

        animation.StopMusic();
        animation.PlayMusic("sounds/gameover.ogg");


        sf::RectangleShape danger;
        danger.setSize(sf::Vector2f(500, 500));
        danger.setPosition(700, 200);
        danger.setFillColor(sf::Color::Red);

        sf::Text over;
        over.setFont(font);
        over.setString("Game over, \n you will be redirected to main menu if you dont restart \n");
        over.setCharacterSize(35);
        over.setFillColor(sf::Color::White); 
        sf::Text score;
        score.setFont(font);
        score.setString("Your highest score :" +currentscore);
        score.setCharacterSize(35);
        score.setFillColor(sf::Color::White);

        // Center the text on the button
        sf::FloatRect textRect = over.getLocalBounds();
        over.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        over.setPosition(sf::Vector2f(danger.getPosition().x + danger.getSize().x / 2.0f, danger.getPosition().y + danger.getSize().y / 2.0f));
        reason.setOrigin(textRect.left + textRect.width / 2.0f +150, textRect.top + textRect.height / 2.0f +150);
        reason.setPosition(sf::Vector2f(danger.getPosition().x + danger.getSize().x / 2.0f + 200, danger.getPosition().y + danger.getSize().y / 2.0f +350));
        score.setPosition(sf::Vector2f(danger.getPosition().x + danger.getSize().x / 2.0f + 200, danger.getPosition().y + danger.getSize().y / 2.0f + 150));
        window.draw(over); // Draw text after the button so it appears on top     
        window.draw(reason);
        window.draw(score);
        grid= std::make_unique<Grid>(getlevel());
        menu.addItem("restart", [this]() {this->setMovesLeft(30), this->setScore(0), gameState = GameState::Playing; });
        menu.draw();
        window.display();
        }

	}

}

void Game::loadGame() {
    std::list<std::string> li;
    li = scoreboard.displayScores(window, *fileManager);
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return; // Ensure the font is loaded successfully
    }


    std::vector<sf::Text> texts; // Store text objects to check for clicks later
    float yPos = 200.0f; // Starting y position for the first score

    if (li.empty()) {
		sf::Text text;
		text.setFont(font);
		text.setString("No saved games found click to play new game");
		text.setCharacterSize(40);
		text.setFillColor(sf::Color::Red);
		text.setPosition(50.0f, yPos);
		texts.push_back(text); // Add to vector for click detection
		yPos += 30.0f; // Increment y position for next score
	}
    else {
        for (const auto& player : li) {
            sf::Text text;
            text.setFont(font);
            text.setString(player);
            text.setCharacterSize(60);
            text.setFillColor(sf::Color::White);
            text.setPosition(50.0f, yPos);
            texts.push_back(text); // Add to vector for click detection
            yPos += 60.0f; // Increment y position for next score
        }
    }


        // Draw texts to the window
        window.clear();
        for (auto& text : texts) {


            window.draw(text);

        }


        window.display();

        // Event handling for selecting a player name
        sf::Event event;
        while (window.waitEvent(event)) { // Use waitEvent for efficiency

            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Get mouse position relative to the window and convert it to world coordinates
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                for (auto& text : texts) {
                    // Check if the sf::Text object's bounding box contains the mouse position
                    if (text.getGlobalBounds().contains(mousePos)) {
                        std::string playerName = text.getString(); // Get the selected player's name
                        if (text.getGlobalBounds().contains(mousePos)) {
                            text.setFillColor(sf::Color::Red);
                        };
                        try {
                            fileManager->loadFromFile(playerName, *this); // Assuming this function exists and is correct
                            gameState = GameState::loading;
                            return;
                        }
                        catch (const std::exception& e) {
                            std::cerr << "Exception occurred while loading file: " << e.what() << std::endl;
                        }
                    };

                
            }

        }
    }
}



void Game::showDifficultyMenu() {
    menu.clearItems(); // Clears the main menu items

    // Add new items for difficulty selection
    menu.addItem("Easy", [this]() { std::make_unique<Grid>("Easy"); gameState = GameState::Playing, setlevel("Easy"), setMovesLeft(30); });
    menu.addItem("Medium", [this]() { std::make_unique<Grid>("Medium"); gameState = GameState::Playing, setlevel("Medium"), setMovesLeft(15); });
    menu.addItem("Hard", [this]() { std::make_unique<Grid>("Hard"); gameState = GameState::Playing, setlevel("Hard"), setMovesLeft(12); });
    menu.addItem("Return", [this]() {gameState= GameState::MainMenu; });



}

void Game::Pause() {
    menu.clearItems(); // Clears the main menu items

    // Add new items for difficulty selection
    menu.addItem("Resume", [this]() { gameState = GameState::Playing; });
    menu.addItem("save game", [this]() { gameState = GameState::Saving; });
    menu.addItem("return", [this]() { gameState = GameState::MainMenu; });
    menu.addItem("exit", [this]() { this->exitGame(), gameState = GameState::MainMenu; });

}