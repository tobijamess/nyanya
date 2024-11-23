#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "enemy.h"

class Player {
public:
	// index gets multiplied by 64 to find the position of single sprite in the spritesheet
	// index is the count of sprites in the sheet, count starts at 0
	int xIndex = 0;
	int yIndex = 0;
	// create sprite object to turn image into a sprite which can be moved, rotated, scaled etc.
	sf::Sprite sprite;
	// create circle shape to use as hitbox
	sf::CircleShape hitbox;
	// set width and height for player sprite
	sf::Vector2i size;
	// player speed value
	float playerSpeed = 500.0f;
	bool isMoving;
	
	
private:
	// create texture object to hold .png image
	sf::Texture texture;
	sf::Vector2f centerOffset;
public:
	void Initialize();
	void Load();
	void Update(Player& player, Enemy& enemy, float deltaTime);
	void Draw(sf::RenderWindow& window);
	void move(const sf::Vector2f& offset);
private:

};
#endif