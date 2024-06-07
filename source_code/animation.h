#pragma once
#ifndef _animation_h_
#define _animation_h_
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <SFML/Audio.hpp>
#include <vector>
#include "Tile.h"
#include <functional>

// Classe Animation : Gère les animations et la musique dans le jeu.
class Animation // Class créée par Rsaissi Mohamed
{
public:
    // Constructeur : Initialise les animations.
    Animation();

    // Affiche une image à une position spécifiée dans la fenêtre de rendu.
    void showImageAt(sf::RenderWindow& window, float x, float y,std::string image);

    // Anime le menu principal avec une séquence d'images.
    void MainMenuAnim(sf::Sprite* sprite, sf::Texture* texture, std::string file, int frames);

    // Sélectionne la frame suivante dans une animation, basée sur le nombre total de frames.
    std::string switchFrame(int frames);

    // Joue un fichier musical.
    void PlayMusic(std::string sound);

    // Arrête la musique en cours.
    void StopMusic();

private:
    // Temporisateur pour le menu principal.
    int MMTimer;

    // Numéro de la frame actuelle.
    int fnum;

    // Gestionnaire de musique pour jouer des sons.
    sf::Music music;
};


#endif // _animation_h_








