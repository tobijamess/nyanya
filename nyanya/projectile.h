#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include "player.h"

class Projectile
{
public:
	// create texture object
	sf::Texture texture;
	// create sprite object
	sf::Sprite sprite;
	// projectile placeholder hitbox
	sf::RectangleShape boundingRectangle;
	// size vector to get size of texture png in pixels
	sf::Vector2i projSize;

	// struct to store every new projectile and its' set direction
	struct ProjectileData {
		sf::Sprite projectile;
		sf::Vector2f direction;
	};
	// Vector2f to store each projectiles final direction
	sf::Vector2f projectileDirection;
private:
	int xIndex = 0;
	int yIndex = 0;
	// create stdlib vector to store list of instanciated structs which hold each new projectile and its' direction
	std::vector<ProjectileData> playerProjectiles;
	// set projectile speed
	float projectileSpeed = 1200.0f;
public:
	void Initialize();
	void Load();
	void Update(sf::RenderWindow& window, Player& player, float deltaTime);
	void Draw(sf::RenderWindow& window);
private:
};