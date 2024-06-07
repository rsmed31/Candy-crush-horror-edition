#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <string>


enum class CandyType { NONE = -1, GREEN, PURPLE, ORANGE, YELLOW, RED, BLUE, EXPLOSIVE };


// Classe Tile : Represente une tuile individuelle dans la grille du jeu.
class Tile { //classe creee par Rsaissi Mohamed
    sf::Color color; // Couleur de la tuile.
    sf::Texture texture; // Texture appliquee a la tuile.
    sf::Sprite sprite; // Sprite utilisant la texture pour l'affichage.
    CandyType candyType = CandyType::NONE; // Type de bonbon represente par la tuile, NONE par defaut.
    bool empty; // Indique si la tuile est vide.

public:
    // Constructeur : initialise la tuile comme non vide avec une couleur blanche par defaut.
    Tile() : color(sf::Color::White), empty(false) {}

    // Dessine la tuile sur le cible de rendu specifiee a la position donnee.
    void draw(sf::RenderTarget& target, sf::Vector2f position) const;

    // Definit le type de bonbon de la tuile.
    void setCandyType(CandyType type);

    // Obtient le type de bonbon de la tuile.
    CandyType getCandyType() const;

    // Verifie si la tuile est vide.
    bool isEmpty() const;

    // Obtient la couleur de la tuile.
    sf::Color getColor() const;

    // Obtient la position actuelle de la tuile.
    sf::Vector2f getPosition() const;

    // Definit la position de la tuile.
    void setPosition(sf::Vector2f position);

    // Charge une texture depuis un fichier et l'applique a la tuile.
    bool loadTexture(const std::string& filename);

    // Marque la tuile comme vide.
    void clear();

    // Marque la tuile comme non vide.
    void unClear();

    // Obtient l'etat de la tuile sous forme de chaine de caracteres.
    std::string getState() const;

    // Definit l'etat de la tuile a partir d'une chaine de caracteres.
    void setState(const std::string& state);

    // Definit la couleur de la tuile.
    void setColor(const sf::Color& newcolor);

    // Obtient la texture appliquee a la tuile.
    sf::Texture gettexture() const;
};




#endif // TILE_H
