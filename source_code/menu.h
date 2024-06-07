#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#pragma once
#ifndef _menu_h_
#define _menu_h_

#include <map>
#include <functional>


// Classe Menu : Gere l'affichage et la navigation dans le menu du jeu.
class Menu { // classe creee par Zouine Mohamed Reda
private:
    sf::RenderWindow& window; // Reference a la fenetre de rendu pour l'affichage
    sf::Font font; // Police utilisee pour le texte du menu
    std::vector<sf::Text> menuItems; // Elements du menu sous forme de texte
    int selectedItemIndex; // Index de l'element du menu actuellement selectionne

public:
    // Constructeur : Initialise le menu avec une reference a la fenetre de rendu SFML
    Menu(sf::RenderWindow& win);

    // Associe des actions aux etiquettes des elements du menu
    std::map<std::string, std::function<void()>> actions;

    // Affiche les elements du menu dans la fenetre
    void draw();

    // Navigue a travers les elements du menu, -1 pour monter, 1 pour descendre
    void navigate(int);

    // Gere les entrees utilisateur (mise a jour de la signature de la methode)
    bool handleInput(const sf::Event& event);

    // Vide la liste des elements du menu
    void clearItems();

    // Ajoute un element au menu avec une etiquette et une action associee
    void addItem(const std::string& label, std::function<void()> action);

    // Ajoute ou met a jour une action associee a une etiquette
    void addAction(const std::string& label, std::function<void()> action) {
        actions[label] = action; // Ajoute ou met a jour une action
    }
};





#endif