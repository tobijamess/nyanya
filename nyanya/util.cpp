#include "util.h"
#include <cmath>

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

// collision detection for circle hitbox
bool Util::collisionDetection(Player& player, Enemy& enemy) {
	sf::Vector2f direction;
	// target - current to find direction vector pointing from player to enemy origin point (use getPosition() because getOrigin() is a relative point within the shape not coordinates)
	direction = enemy.hitbox.getPosition() - player.hitbox.getPosition();
	// get magnitude or m by getting the square root of direction.x^2 + direction.y^2 (magnitude is the length)
	float m = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	// if m (length) less than or equal to 64, collision is happening (because I set both circles radius to 32)
	if (m <= 64) {
		return true;
	}
	else {
		return false;
	}
}

// PUSHBACK NEEDS FIXING CURRENTLY CAN TELEPORT PLAYER SPRITE WHEN THEY COLLIDE???
// collision direction and pushback (opposite force that is applied when two hitboxes collide)
bool Util::collisionDirection(Player& player, Enemy& enemy) {
	sf::Vector2f direction;
	// if collisionDetection returned true, execute code
	if (Util::collisionDetection(player, enemy)) {
		// get direction vector (target - current)
		direction = enemy.hitbox.getPosition() - player.hitbox.getPosition();
		// get magnitude (m)
		float m = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		// normalize direction vector
		direction = Util::normalizeVector(direction);
		// if m is not equal to 0, execute code (don't want to divide by 0)
		if (m != 0.0f) {
			direction.x /= m;
			direction.y /= m;
		}
		// set force of pushback from collision direction
		float force = 1000.0f;
		// move hitbox in opposite direction of collision
		player.move(direction * force);
		return true;
	}
}

// function to calculate the rotation based on the normalizedVector returned by the normalizeVector() function
float Util::calculateRotation(sf::Vector2f vector) {
	// use atan2 instead of atan because atan assumes that both x and y are of the same sign (positive or negative) which means the angle will be limited to a 180° range
	// atan2 accounts for the normalized vectors x and y's individual signs meaning the angle can cover a full 360° and wont be limited to either 2 positive or 2 negative quadrants
	float radians = atan2(vector.y, vector.x);
	// get rotation degrees by multiplying the radians by 180 and then dividing by pi
	float degrees = radians * (180.0f / 3.14159265359f);
	return degrees;
}

// function to check collision between two different rectangles (check paint for explanation)
//bool Util::rectCollision(sf::FloatRect rect1, sf::FloatRect rect2) {
//	if (rect1.left + rect1.width > rect2.left &&
//		rect2.left + rect1.width > rect1.left &&
//		rect2.top + rect2.height > rect1.top &&
//		rect1.top + rect1.height > rect2.top) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}