#include <iostream>
#include <string>
#include "enemy.h"

void Enemy::Initialize() {

}

void Enemy::Load() {
	if (texture.loadFromFile("assets/enemy/textures/spritesheet.png")) {
		std::cout << "enemy image loaded\n";
		sprite.setTexture(texture);
		int xIndex = 0;
		int yIndex = 0;
		sprite.setTextureRect(sf::IntRect(xIndex * 64, yIndex * 64, 64, 64));
		sprite.scale(sf::Vector2f(3, 3));
	}
	else {
		std::cout << "enemy image didnt load\n";
	}
}

void Enemy::Update() {

}

void Enemy::Draw(sf::RenderWindow& window) {
	window.draw(sprite);
}