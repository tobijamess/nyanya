#include <iostream>
#include "player.h"
#include "util.h"

void Player::Initialize() {
	// initialize and set the size of playerSprite and boundingBox to 64x64 (pixel size of sprite)
	size = sf::Vector2i(64, 64);
	// initialize/set features for the boundingRectangle(hitbox)
	boundingRectangle.setFillColor(sf::Color::Transparent);
	boundingRectangle.setOutlineColor(sf::Color::Red);
	boundingRectangle.setOutlineThickness(1);
}

// function to load player stuff like sprites, set default sprite position for movement etc.
// pass Player object as parameter to access Player members and change them
void Player::Load() {
	// load texture from file into texture object
	if (texture.loadFromFile("assets/player/textures/playerspritesheet.png")) {
		// successful load
		std::cout << "player image loaded\n";
		// use setTexture function to set the sprite to texture (which holds the .png)
		sprite.setTexture(texture);
		// function to put rectangle (sf::IntRect) over the pixels you want to select for drawing within the texture by setting x, y and width/height (all integers)
		// xIndex is sprite position in the row, yIndex is sprite position in column, is multiplied by 64 so the rectangle can move over/down 1 sprite at a time
		// all player sprites are 64x64 so rectWidth and rectHeight stay at 64, 64
		sprite.setTextureRect(sf::IntRect(xIndex * size.x, yIndex * size.y, size.x, size.y));
		sprite.setPosition(sf::Vector2f(800, 500));
		// multiplies the current scale of sprite object (make sprite bigger)
		sprite.scale(sf::Vector2f(3, 3));
		// set hitbox size to same size as sprite (64x64px) and multiply by a smaller scale (2), not sprite.getScale() to make it closer to actual sprite size
		boundingRectangle.setSize(sf::Vector2f(size.x * sprite.getScale().x, size.y * sprite.getScale().y));
	}
	else {
		// failed load
		std::cout << "player image didnt load\n";
	}
	return;
}

// function to take player input and change position of player sprite based on input
// pass Player object as parameter to access Player members and change them
void Player::Update(Enemy& enemy, float deltaTime) {

	// create movement vector2f to store the values produced by the keyboard presses which are based on the x and y axis
	sf::Vector2f movement(0.0f, 0.0f);
	// check each of the movement keys for presses and adjust the movement vector2f
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		// movement.y = movement.y - 1.0f
		movement.y -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		movement.x -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		movement.y += 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		movement.x += 1.0f;
	}
	// condition ensures that normalization only happens if there actually is movement
	if (movement.x != 0.0f || movement.y != 0.0f) {
		// normalize the movement vector
        movement = Util::normalizeVector(movement);
    }
	// set sprites position
	sprite.setPosition(sprite.getPosition() + movement * playerSpeed * deltaTime);

	// set boundingRectangle (hitbox) to sprites position so it always follows player
	boundingRectangle.setPosition(sprite.getPosition());
	// check for collision
	if (Util::rectCollision(sprite.getGlobalBounds(), enemy.sprite.getGlobalBounds())) {
		std::cout << "collision working\n";
	}
}

// function to draw player sprite to the window
// pass Player & RenderWindow class object's as parameters so draw function can draw the texture to the render window
void Player::Draw(sf::RenderWindow& window) {
	// draw the sprite to window, only sprites can be drawn not textures
	window.draw(sprite);
	window.draw(boundingRectangle);
}