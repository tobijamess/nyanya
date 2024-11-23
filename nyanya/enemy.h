#ifndef ENEMY_H
#define ENEMY_H
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

// see player.h for commenting
class Enemy {
public:
	int xIndex = 0;
	int yIndex = 0;

	sf::Sprite sprite;
	sf::CircleShape hitbox;
	sf::Vector2i size;
private:
	float enemySpeed = 150.0f;
	sf::Texture texture;
public:
	void Initialize();
	void Load();
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);
private:
};
#endif 