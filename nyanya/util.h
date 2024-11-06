#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <math.h>



class Util {
public:
	static sf::Vector2f normalizeVector(sf::Vector2f vector);
	static bool rectCollision(sf::FloatRect rect1, sf::FloatRect rect2);
	static float calculateRotation(sf::Vector2f vector);
private:
};