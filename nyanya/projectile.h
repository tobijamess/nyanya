#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include "player.h"

class Projectile
{
public:
	// create objects to be used in all the playerProjectile functions
	std::vector<sf::RectangleShape> playerProjectiles;
	sf::Vector2f projectileDirection;
	float projectileSpeed = 0.5f;
private:
public:
	void Initialize();
	void Load();
	void Update(sf::RenderWindow& window, Player& player);
	void Draw(sf::RenderWindow& window);
private:
};