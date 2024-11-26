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
	float detectionRadius = 300.0f;
	sf::CircleShape detectionCircle;
	float attackRadius = 100.0f;
	sf::CircleShape attackCircle;
public:
	void Initialize();
	void Load();
	void Update(float deltaTime, const sf::Vector2f& playerPos);
	void Draw(sf::RenderWindow& window);

	void SetDetectionRadius(float radius);
	void SetAttackRadius(float radius);
	bool IsPlayerInRange(const sf::Vector2f& playerPos, float radius);
private:
};
#endif 