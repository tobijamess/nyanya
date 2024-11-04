#include "enemy.h"

void enemyLoad(EnemyClass& enemy) {
	if (enemy.enemyTexture.loadFromFile("assets/enemy/textures/enemyspritesheet.png")) {
		std::cout << "enemy image loaded\n";
		enemy.enemySprite.setTexture(enemy.enemyTexture);
		int xIndex = 0;
		int yIndex = 0;
		enemy.enemySprite.setTextureRect(sf::IntRect(xIndex * 64, yIndex * 64, 64, 64));
		enemy.enemySprite.scale(sf::Vector2f(3, 3));
	}
	else {
		std::cout << "enemy image didnt load\n";
	}
}
void enemyMove(EnemyClass& enemy) {

}
void enemyDraw(EnemyClass& enemy, sf::RenderWindow& window) {
	window.draw(enemy.enemySprite);
}