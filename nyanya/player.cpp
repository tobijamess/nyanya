#include <iostream>
#include "player.h"
#include "util.h"

// function to combine all player movement variables as well as collision
void Player::move(const sf::Vector2f& offset) {
	// adjust the sprite position based on the offset
	sprite.setPosition(sprite.getPosition() + offset);
	// Update hitbox position to match the sprite
	hitbox.setPosition(sprite.getPosition());
}

void Player::Initialize() {
	// initialize and set the size of playerSprite to 64x64 (pixel size of sprite)
	size = sf::Vector2i(64, 64);
	// initialize/set features for the hitbox(circleshape)
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineColor(sf::Color::Red);
	hitbox.setOutlineThickness(1);
	// set radius to half player sprite size so diameter is same as sprite's height and width (64x64)
	hitbox.setRadius(32);
	// set origin of hitbox to the center of the sprite image, not the sprite origin
	hitbox.setOrigin(-64, -64);
}

// function to load player stuff like sprites, set default sprite position for movemsent etc.
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
	}
	else {
		// failed load
		std::cout << "player image didnt load\n";
	}
	return;
}

// function to take player input and change position of player sprite based on input
// pass Player object as parameter to access Player members and change them
void Player::Update(Player& player, Enemy& enemy, float deltaTime) {
	// create movement vector2f to store the values produced by the keyboard presses which are based on the x and y axis
	sf::Vector2f movement(0.0f, 0.0f);
	
	// check each of the movement keys for presses and adjust the movement vector2f
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		// movement.y = movement.y - 1.0f
		movement.y -= 1.0f;
		// get sprite index for each movement direction
		xIndex = 0;
		yIndex = 3;
		// set the sprite state based on keypressed
		sprite.setTextureRect(sf::IntRect(xIndex * size.x, yIndex * size.y, size.x, size.y));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		movement.x -= 1.0f;
		xIndex = 0;
		yIndex = 1;
		sprite.setTextureRect(sf::IntRect(xIndex * size.x, yIndex * size.y, size.x, size.y));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		movement.y += 1.0f;
		xIndex = 0;
		yIndex = 0;
		sprite.setTextureRect(sf::IntRect(xIndex * size.x, yIndex * size.y, size.x, size.y));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		movement.x += 1.0f;
		xIndex = 0;
		yIndex = 2;
		sprite.setTextureRect(sf::IntRect(xIndex * size.x, yIndex * size.y, size.x, size.y));
	}
	// condition ensures that normalization only happens if there actually is movement
	if (movement.x != 0.0f || movement.y != 0.0f) {
		// normalize the movement vector
		movement = Util::normalizeVector(movement);
	}
	// bool variable to check if the player is moving for use in util.cpp collision function
	bool isMoving = (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
					 sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D));

	// call move function with all movement and collision variables taken into account
	move(movement * playerSpeed * deltaTime);
	// set hitbox to sprites position so it always follows player
	hitbox.setPosition(sprite.getPosition().x, sprite.getPosition().y);
	// check for collision
	if (Util::playerCollision(player, enemy, deltaTime, isMoving)) {
		std::cout << "collision working\n";
	}
}

// function to draw player sprite to the window
// pass Player & RenderWindow class object's as parameters so draw function can draw the texture to the render window
void Player::Draw(sf::RenderWindow& window) {
	// draw the sprite to window, only sprites can be drawn not textures
	window.draw(sprite);
	window.draw(hitbox);
}