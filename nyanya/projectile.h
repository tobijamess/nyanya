#ifndef PROJECTILE_H
#define PROJECTILE_H
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
	// size vector to get size of texture png in pixels
	sf::Vector2i projSize;

	// timer to prevent too many projectiles being created every frame
	float cooldown = 0.25f;
	float timeSinceLastProjectile = 0.0f;


	// struct to store every new projectile and its' set direction
	struct ProjectileData {
		sf::CircleShape hitbox;
		sf::Sprite projectile;
		sf::Vector2f direction;
		float lifetime;
	};
	// struct object to access struct variables in projectile.cpp functions
	ProjectileData projData;
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
	void Load(Player& player);
	void Update(sf::RenderWindow& window, Player& player, Enemy& enemy, float deltaTime);
	void Draw(sf::RenderWindow& window);

	// getter for vector of instanciated structs to use in collision detection
	const std::vector<Projectile::ProjectileData>& getPlayerProjectiles() const {
		return playerProjectiles;
	}

private:
};
#endif