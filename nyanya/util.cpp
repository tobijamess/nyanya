#include "util.h"

// function to normalize vectors
sf::Vector2f Util::normalizeVector(sf::Vector2f vector) {
	// use sqrt func to get magnitude (m) of the vector using the equation vector.x^2 + vector.y^2 or vector.x * vector.x + vector.y * vector.y
	float m = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	// create normalizedVector object to store the result of the normalization equation
	sf::Vector2f normalizedVector;
	// x axis divided by the magnitude, stored in object.x (normalizedVector.x)
	normalizedVector.x = vector.x / m;
	// y axis divided by the magnitude, stored in object.y (normalizedVector.y)
	normalizedVector.y = vector.y / m;
	// return result to finalize 
	return normalizedVector;
}
// function to check collision between two different rectangles (check paint for explanation)
bool Util::rectCollision(sf::FloatRect rect1, sf::FloatRect rect2) {
	if (rect1.left + rect1.width > rect2.left &&
		rect2.left + rect1.width > rect1.left &&
		rect2.top + rect2.height > rect1.top &&
		rect1.top + rect1.height > rect2.top) {
		return true;
	}
	else {
		return false;
	}
}