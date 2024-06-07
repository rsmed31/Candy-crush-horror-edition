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

// Classe Animation : G�re les animations et la musique dans le jeu.
class Animation // Class cr��e par Rsaissi Mohamed
{
public:
    // Constructeur : Initialise les animations.
    Animation();

    // Affiche une image � une position sp�cifi�e dans la fen�tre de rendu.
    void showImageAt(sf::RenderWindow& window, float x, float y,std::string image);

    // Anime le menu principal avec une s�quence d'images.
    void MainMenuAnim(sf::Sprite* sprite, sf::Texture* texture, std::string file, int frames);

    // S�lectionne la frame suivante dans une animation, bas�e sur le nombre total de frames.
    std::string switchFrame(int frames);

    // Joue un fichier musical.
    void PlayMusic(std::string sound);

    // Arr�te la musique en cours.
    void StopMusic();

private:
    // Temporisateur pour le menu principal.
    int MMTimer;

    // Num�ro de la frame actuelle.
    int fnum;

    // Gestionnaire de musique pour jouer des sons.
    sf::Music music;
};


#endif // _animation_h_








