#ifndef UTIL_H
#define UTIL_H
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <math.h>
#include "player.h"
#include "projectile.h"

class Util {
public:
	/*static bool rectCollision(sf::FloatRect rect1, sf::FloatRect rect2);*/
	static sf::Vector2f normalizeVector(sf::Vector2f vector);
	static float calculateRotation(sf::Vector2f vector);
	// collision detection functions
	static bool collisionDetection(Player& player, Enemy& enemy);
	static bool collisionDetection(const sf::CircleShape& projHitbox, const sf::CircleShape& enemyHitbox);
	// collision outcome functions
	static bool playerCollision(Player& player, Enemy& enemy, float deltaTime, bool isMoving);
private:
};
#endif