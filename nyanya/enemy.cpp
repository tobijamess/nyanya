#include <iostream>
#include <string>
#include "enemy.h"
#include "util.h"

void Enemy::Initialize() {
	// initialize and set the size of enemySprite to 64x64px's
	size = sf::Vector2i(64, 64);
	// initialize/set features for the hitbox which is a circle
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineColor(sf::Color::Blue);
	hitbox.setOutlineThickness(1);
	float scaleFactor = 1.0f; // both x and y scaling factors are the same
	float scaledWidth = size.x * scaleFactor; // width of the scaled fireball
	float scaledHeight = size.y * scaleFactor; // height of the scaled fireball
	float hitboxRadius = std::max(scaledWidth, scaledHeight) / 2.0f; // use max dimension
	hitbox.setRadius(hitboxRadius);
	// set origin of hitbox to the center of the sprite image so the hitbox encompasses the sprite
	hitbox.setOrigin(hitboxRadius, hitboxRadius);
	// initialize detection circles which will determine whether player is in range to be moved towards
	detectionCircle.setRadius(detectionRadius);
	detectionCircle.setOrigin(detectionRadius, detectionRadius);
	detectionCircle.setFillColor(sf::Color::Transparent);
	detectionCircle.setOutlineColor(sf::Color::Green);
	detectionCircle.setOutlineThickness(1);
	// initialize attack circles which will determine whether player is in range to be attacked
	attackCircle.setRadius(attackRadius);
	attackCircle.setOrigin(attackRadius, attackRadius);
	attackCircle.setFillColor(sf::Color::Transparent);
	attackCircle.setOutlineColor(sf::Color::Red);
	attackCircle.setOutlineThickness(1);
}

void Enemy::Load(Type type) {
	// shared texture for all enemies
	static sf::Texture texture;
	// see player.cpp for comments
	if (!texture.loadFromFile("assets/enemy/textures/enemyspritesheet.png")) {
		std::cout << "enemy image failed to load\n";
	}
	else {
		sprite.setTexture(texture);
	}
	int xIndex = 0;
	int yIndex = 0;
	sprite.setTextureRect(sf::IntRect(xIndex * size.x, yIndex * size.y, size.x, size.y));
	sf::Vector2f scaleFactor = sf::Vector2f(3.0f, 3.0f);
	sprite.setScale(scaleFactor);
	sprite.setOrigin(static_cast<float>(size.x) / 2.0f, static_cast<float>(size.y) / 2.0f);
}

void Enemy::Update(float deltaTime, const sf::Vector2f& playerPos) {
	// continuously update hitbox position to sprite position
	hitbox.setPosition(sprite.getPosition().x, sprite.getPosition().y);
	// if player is within detection radius range
	if (IsPlayerInRange(playerPos, detectionRadius)) {
		// if player is not in attack radius range
		if (!IsPlayerInRange(playerPos, attackRadius)) {
			// direction vector to move enemy in
			sf::Vector2f direction = playerPos - sprite.getPosition();
			// normalize direction vector
			direction = Util::NormalizeVector(direction);
			// move enemy with params
			sprite.move(direction * enemySpeed * deltaTime);
		}
	}
}

void Enemy::Draw(sf::RenderWindow& window) {
	// draw enemy sprites, hitboxes etc.
	window.draw(sprite);
	window.draw(hitbox);
	detectionCircle.setPosition(sprite.getPosition());
	attackCircle.setPosition(sprite.getPosition());
	window.draw(detectionCircle);
	window.draw(attackCircle);
}

// function to check whether player is within range of either detection radius or attack radius
bool Enemy::IsPlayerInRange(const sf::Vector2f& playerPos, float radius) {
	sf::Vector2f direction = playerPos - sprite.getPosition();
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	return distance <= radius;
}