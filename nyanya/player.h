#pragma	once
#include <string>
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
	// create boundingRectangle shape to use as hitbox
	sf::RectangleShape boundingRectangle;
	// set width and height for player sprite
	sf::Vector2i size;
	// player speed value
	float playerSpeed = 500.0f;
private:

	// create texture object to hold .png image
	sf::Texture texture;
public:
	void Initialize();
	void Load();
	void Update(Enemy& enemy, float deltaTime);
	void Draw(sf::RenderWindow& window);
private:

};