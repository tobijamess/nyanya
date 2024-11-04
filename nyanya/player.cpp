#include <iostream>
#include "player.h"
#include "util.h"

void Player::Initialize() {

}

// function to load player stuff like sprites, set default sprite position for movement etc.
// pass Player object as parameter to access Player members and change them
void Player::Load() {
	// load texture from file into temptexture object
	if (texture.loadFromFile("assets/player/textures/spritesheet.png")) {
		// successful load
		std::cout << "player image loaded\n";
		// set temptexture object equal to
		// use setTexture function to set the sprite to texture (which holds the .png)
		sprite.setTexture(texture);
		// function to put rectangle (sf::IntRect) over the pixels you want to select for drawing within the texture by setting x, y and width/height (all integers)
		// xIndex is sprite position in the row, yIndex is sprite position in column, is multiplied by 64 so the rectangle can move over/down 1 sprite at a time
		// all player sprites are 64x64 so rectWidth and rectHeight stay at 64, 64
		sprite.setTextureRect(sf::IntRect(xIndex * 64, yIndex * 64, 64, 64));
		// multiplies the current scale of sprite object (make sprite bigger)
		sprite.scale(sf::Vector2f(3, 3));
		sprite.setPosition(sf::Vector2f(800, 500));
	}
	else {
		// failed load
		std::cout << "player image didnt load\n";
	}
	return;
}

// function to take player input and change position of player sprite based on input
// pass Player object as parameter to access Player members and change them
void Player::Update() {
	// create vector2f object ('position', which takes x, y) to store the value (x, y) of sprite.getPosition which is by default 0,0 (top left of window)
	sf::Vector2f position = sprite.getPosition();
	// read the realtime keyboard, check if a key was pressed, if the key pressed was 'W', run the code
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		sprite.setPosition(position + sf::Vector2f(0.00f, -0.1f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		sprite.setPosition(position + sf::Vector2f(-0.1f, 0.00f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		sprite.setPosition(position + sf::Vector2f(0.00f, 0.1f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { 
		sprite.setPosition(position + sf::Vector2f(0.1f, 0.00f));
	}
}

// function to draw player sprite to the window
// pass Player & RenderWindow class object's as parameters so draw function can draw the texture to the render window
void Player::Draw(sf::RenderWindow& window) {
	// draw the sprite to window, only sprites can be drawn not textures
	window.draw(sprite);
}