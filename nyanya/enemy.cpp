#include <iostream>
#include <string>
#include "enemy.h"

void Enemy::Initialize() {
	size = sf::Vector2i(64, 64);
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineColor(sf::Color::Blue);
	hitbox.setOutlineThickness(1);
	hitbox.setOrigin(sprite.getPosition());
	hitbox.setRadius(32);
}

void Enemy::Load() {
	if (texture.loadFromFile("assets/enemy/textures/enemyspritesheet.png")) {
		std::cout << "enemy image loaded\n";
		sprite.setTexture(texture);
		int xIndex = 0;
		int yIndex = 0;
		sprite.setTextureRect(sf::IntRect(xIndex * size.x, yIndex * size.y, size.x, size.y));
		sprite.setPosition(300, 200);
		sprite.scale(sf::Vector2f(3, 3));
	}
	else {
		std::cout << "enemy image didnt load\n";
	}
}

void Enemy::Update(float deltaTime) {
	hitbox.setPosition(sprite.getPosition());
}

void Enemy::Draw(sf::RenderWindow& window) {
	window.draw(sprite);
	window.draw(hitbox);
}