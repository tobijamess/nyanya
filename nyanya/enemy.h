#ifndef ENEMY_H
#define ENEMY_H
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

class Enemy {
public:
	// placeholder enemy types
	enum class Type { Basic, Advanced, Boss };
	int health = 100;
	int xIndex = 0;
	int yIndex = 0;
	sf::Sprite sprite;
	sf::CircleShape hitbox;
	sf::Vector2i size;
	Type type;
	// getter function for use in Game::Initialize
	Type GetType() {
		return type;
	}
private:
	float enemySpeed = 200.0f;
	float detectionRadius = 1000.0f;
	sf::CircleShape detectionCircle;
	float attackRadius = 100.0f;
	sf::CircleShape attackCircle;
public:
	void Initialize();
	void Load(Type type);
	void Update(float deltaTime, const sf::Vector2f& playerPos);
	void Draw(sf::RenderWindow& window);

	/*void SetDetectionRadius(float radius);
	void SetAttackRadius(float radius);*/
	bool IsPlayerInRange(const sf::Vector2f& playerPos, float radius);
private:
};
#endif 