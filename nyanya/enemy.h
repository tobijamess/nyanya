#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

// see player.h for commenting
class EnemyClass {
public:

	std::string enemyName = "enemyName";
	int enemyHealth = 100;
	int enemyDamage = 0;
	float enemyAttackSpeed = 0.0f;

	int xIndex = 0;
	int yIndex = 0;

	sf::Texture enemyTexture;
	sf::Sprite enemySprite;

private:


};

void enemyLoad(EnemyClass& enemy);
void enemyMove(EnemyClass& enemy);
void enemyDraw(EnemyClass& enemy, sf::RenderWindow& window);