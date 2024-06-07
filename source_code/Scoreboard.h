#include <iostream>
#include <vector>
#include "Player.h"
#include <SFML/Graphics.hpp>
#include "FileManager.h"
#include <list>

#pragma once
#ifndef _scoreboard_h_
#define _scoreboard_h_

// Classe Scoreboard : Gere l'affichage des scores eleves dans le jeu.
class Scoreboard { //classe cree par Zouine Mohamed Reda
    std::vector<Player> highScores; // Vecteur contenant les joueurs avec leurs scores eleves.

public:
    // Affiche les scores eleves dans une fenetre de rendu donnee et utilise FileManager pour recuperer les donnees.
    std::list<std::string> displayScores(sf::RenderWindow& window, FileManager filemanager);
};


#endif