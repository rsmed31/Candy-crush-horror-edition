#include "filemanager.h" 

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Grid.h"
#include "Player.h"
#include "Scoreboard.h"
#include <memory>
#include "animation.h"


enum class GameState {
    MainMenu,
    SelectDifficulty,
    Playing,
    Paused,
    Saving, 
    GameOver,
    Scoreboard,
    loading,
    iload
};

// Classe Game : Gere le deroulement global du jeu.
class Game { // Principalement cree par Rsaissi Mohamed (et Zouine Mohamed)
public:
    Game(); // Constructeur
    void run(); // Lance le cycle principal du jeu
    void startGame(); // Initie une nouvelle partie
    void endGame(); // Gere la fin d'une partie
    int getScore() const { return score; } // Accede au score actuel
    void setScore(int newScore) { score = newScore; } // Definit le score
    int getMovesLeft() const { return movesLeft; } // Accede au nombre de mouvements restants
    void setMovesLeft(int newMovesLeft) { movesLeft = newMovesLeft; } // Definit les mouvements restants
    void setlevel(std::string newlevel) { grid->setdifficulty(newlevel); } // Definit le niveau de difficulte

    void loadGame(); // Charge l'etat de jeu depuis un fichier
    void showDifficultyMenu(); // Affiche le menu de selection de difficulte
    void setupMenuActions(); // Configure les actions du menu
    void exitGame(); // Quitte le jeu
    void Pause(); // Met le jeu en pause
    void setplayername(std::string name, int score_); // Definit le nom du joueur et son score
    void saveGame(); // Sauvegarde l'etat de jeu dans un fichier
    std::string getplayername() { return player->getName(); } // Obtient le nom du joueur
    int gridwidth() { return grid->getgridwidth(); } // Accede a la largeur de la grille
    int gridheight() { return grid->getgridheight(); } // Accede a la hauteur de la grille
    std::string getTileState(int x, int y) const {
        if (!grid) return ""; // Verifie si la grille est initialisee
        return grid->getTileState(x, y);
    }
    std::string getlevel() { return grid->getdifficulty(); }; // Obtient le niveau de difficulte
    void setGrid(Grid* grid) {
        this->grid = std::unique_ptr<Grid>(grid);
    }

private:
    sf::RenderWindow window; // Fenetre de rendu SFML
    Menu menu; // Menu du jeu
    std::unique_ptr<FileManager> fileManager; // Gestionnaire de fichiers
    std::unique_ptr<Grid> grid; // Grille du jeu
    std::unique_ptr<Player> player; // Joueur
    Scoreboard scoreboard; // Tableau des scores
    Animation animation; // Objet d'animation

    int score; // Score actuel du jeu
    int movesLeft; // Mouvements restants dans la session de jeu actuelle
    GameState gameState; // Etat actuel du jeu

    void update(int newscore); // Met a jour l'etat du jeu
    void processEvents(sf::Vector2u windowsize); // Traite les entrees utilisateur et autres evenements
};


#endif // GAME_H
