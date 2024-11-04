#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

class PlayerClass {
public:

	std::string playerName = "playerName";
	int playerHealth = 100;
	int playerDamage = 0;
	float playerAttackSpeed = 0.0f;

	// index gets multiplied by 64 to find the position of single sprite in the spritesheet
	// index is the count of sprites in the sheet, count starts at 0
	int xIndex = 0;
	int yIndex = 0;

	// create playerTexture object to hold .png image
	sf::Texture playerTexture;
	// create playerSprite object to turn image into a sprite which can be moved, rotated, scaled etc.
	sf::Sprite playerSprite;

	// create objects to be used in all the playerProjectile functions
	std::vector<sf::RectangleShape> playerProjectiles;
	sf::Vector2f projectileDirection;
	float projectileSpeed;

private:


};

void playerLoad(PlayerClass& player);
void playerMove(PlayerClass& player);
void playerDraw(PlayerClass& player, sf::RenderWindow& window);

void playerProjectileLoad(PlayerClass& player);
void playerProjectileMove(PlayerClass& player, sf::RenderWindow& window);
void playerProjectileDraw(PlayerClass& player, sf::RenderWindow& window);
