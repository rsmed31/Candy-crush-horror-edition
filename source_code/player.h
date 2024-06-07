#include <iostream>
#include <string> 

#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

// Classe Player : Represente un joueur dans le jeu.
class Player {  // class creee par Zouine Mohamed Reda
    std::string playerName; // Nom du joueur
    int currentScore; // Score actuel du joueur

public:
    // Constructeur initialise le joueur avec un nom et definit le score initial a 0
    Player(const std::string& name) : playerName(name), currentScore(0) {}

    // Methode pour mettre a jour le score du joueur
    void updateScore(int score) {
        currentScore += score; // Ajoute le score fourni au score actuel
    }

    // Methode d'acces pour obtenir le nom du joueur
    std::string getName() const {
        return playerName;
    }

    // Methode d'acces pour obtenir le score actuel du joueur
    int getScore() const {
        return currentScore;
    }

    // Methode pour definir le nom du joueur
    void setname(std::string name) {
        playerName = name;
    }

    // Methode pour definir le score du joueur
    void setscore(int score) {
        currentScore = score;
    }
};


#endif // _PLAYER_H_