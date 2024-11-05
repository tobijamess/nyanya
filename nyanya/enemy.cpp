#include <iostream>
#include <string>
#include "enemy.h"
 
void Enemy::Initialize() {
	size = sf::Vector2i(64, 64);
	boundingRectangle.setFillColor(sf::Color::Transparent);
	boundingRectangle.setOutlineColor(sf::Color::Blue);
	boundingRectangle.setOutlineThickness(1);
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
		boundingRectangle.setSize(sf::Vector2f(size.x * sprite.getScale().x, size.y * sprite.getScale().y));
	}
	else {
		std::cout << "enemy image didnt load\n";
	}
}

void Enemy::Update(float deltaTime) {
	boundingRectangle.setPosition(sprite.getPosition());
}

void Enemy::Draw(sf::RenderWindow& window) {
	window.draw(sprite);
	window.draw(boundingRectangle);
}