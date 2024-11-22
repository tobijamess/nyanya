#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <math.h>
#include "player.h"

class Util {
public:
	/*static bool rectCollision(sf::FloatRect rect1, sf::FloatRect rect2);*/
	static sf::Vector2f normalizeVector(sf::Vector2f vector);
	static float calculateRotation(sf::Vector2f vector);
	static bool collisionDetection(Player& player, Enemy& enemy);
	static bool collision(Player& player, Enemy& enemy, float deltaTime, bool isMoving);
	//static bool projCollision(Projectile& projectile, Enemy& enemy, float deltaTime);
private:
};