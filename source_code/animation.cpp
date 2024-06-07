#include "Animation.h"
#include <iostream>
#include "Tile.h"

Animation::Animation() {
    MMTimer = 12;
    fnum = 1;  // Start from frame 1
    music.openFromFile("sounds/background.ogg");  // Load the music file
    music.setLoop(true);  // Set the music to loop
}


void Animation::MainMenuAnim(sf::Sprite* sprite, sf::Texture* texture,std::string file, int frames)
{
    if (fnum <= frames)  
    {
        if (MMTimer == 0)  // Check if the timer has reached 0
        {
            std::string frame = switchFrame(frames-1);  // Get the next frame string
            texture->loadFromFile("images/" + file +"/" + frame + ".png");
            sprite->setTexture(*texture);
            MMTimer = 4;  // Reset the timer
        }
        else
        {
            MMTimer--;  // Decrement the timer
        }
            sprite->setPosition(50, 50);
        
    }
    else
    {
        fnum = 1;  // Reset to frame 1 after reaching frame 30
    }
}

std::string Animation::switchFrame(int frames)
{
    std::string frameNumber = std::to_string(fnum);  // Convert frame number to string
    fnum++;  // Move to the next frame
    if (fnum > frames) {
        fnum = 1;  // Loop back to frame 1 after frame 30
    }
    return "frame" + frameNumber;  // Return the frame file name
}

void Animation::PlayMusic(std::string sound) {
    if (!music.openFromFile(sound)) {
		std::cerr << "Failed to load music" << std::endl;
		return;
	}
    music.play();
}
void Animation::StopMusic() {
    music.setLoop(false);  // Stop the music from looping (optional
	music.stop();
}


void Animation::showImageAt(sf::RenderWindow& window, float x, float y,std::string image) {
    sf::Texture texture;
    if (!texture.loadFromFile(image)) {
        // Handle error if image loading fails
        return;
    }

    sf::Sprite sprite(texture);

    // Adjust position to center the sprite at the given coordinates
    sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
    sprite.setPosition(x, y);

    // Scale the texture to 50x50
    sprite.setScale(150.f / texture.getSize().x, 150.f / texture.getSize().y);

    window.draw(sprite);
    window.display();

    // Wait for 200 milliseconds
    sf::sleep(sf::milliseconds(200));

    // Fade out
    for (int i = 255; i >= 0; i -= 5) {
        sprite.setColor(sf::Color(255, 255, 255, i));
        window.draw(sprite);
        window.display();
        sf::sleep(sf::milliseconds(10)); // Adjust the duration of fading
    }
}


