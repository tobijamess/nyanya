
#include "util.h"
#include <cmath>

// function to normalize vectors
sf::Vector2f Util::NormalizeVector(sf::Vector2f vector) {
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

// player -> enemy collision detection
bool Util::CollisionDetection(Player& player, Enemy& enemy) {
	sf::Vector2f direction;
	// target - current to find direction vector pointing from player to enemy origin point (use getPosition() because getOrigin() is a relative point within the shape not coordinates)
	direction = enemy.hitbox.getPosition() - player.hitbox.getPosition();
	// get magnitude or m by getting the square root of direction.x^2 + direction.y^2 (magnitude is the length)
	float m = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	// if m (length) less than or equal to 64, collision is happening (because I set both circles radius to 32)
	// check if the distance is less than or equal to the sum of the radii
	float combinedRadius = player.hitbox.getRadius() + enemy.hitbox.getRadius();
	if (m <= combinedRadius) {
		return true; // Collision detected
	}
	else {
		return false;
	}
}

// collision direction and pushback (opposite force that is applied when two hitboxes collide)
bool Util::PlayerCollision(Player& player, Enemy& enemy, float deltaTime, bool isMoving) {
	// initalize direction vector
	sf::Vector2f direction;
	// Check if collision is occurring
	if (Util::CollisionDetection(player, enemy)) {
		// target - current to find direction vector
		direction = enemy.hitbox.getPosition() - player.hitbox.getPosition();
		// get the maginute (length) between player and enemy and store in distance variable
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		// normalize the direction vector
		direction = Util::NormalizeVector(direction);
		// minimum distance which the player and enemy need to be kept apart at (sum of the player + enemy hitbox radius)
		float collisionBoundary = 64.0f;
		// multiply direction by collisionBoundary to get a 64 length vector (direction has length of 1 because it was normalized) that points from enemys center point out toward player
		// subtracting the resulting vector from enemy.hitbox.getPosition() places boundaryPosition exactly at 64 units away from the enemies center in the opposite direction of the player
		sf::Vector2f boundaryPosition = enemy.hitbox.getPosition() - direction * collisionBoundary;
		// if distance between player and enemy is less than the boundary, re-position the player at boundaryPosition which is exactly 64 units away from the enemy center point
		if (distance < collisionBoundary) {
			player.hitbox.setPosition(boundaryPosition);
			player.sprite.setPosition(boundaryPosition);
		}
		// if player is moving, allow sliding along the boundary
		if (isMoving) {
			// calculate the tangent vector for sliding around the boundary (tangent vector is a vector perpandicular to direction)
			// tangent vector is rotated 90 degrees by swapping directions x and y values and negating one of them (hence -y, x instead of the normal x, y)
			sf::Vector2f tangent(-direction.y, direction.x);
			sf::Vector2f slideAround = tangent * player.playerSpeed * deltaTime;
			player.Move(slideAround);
		}
		return true;
	}
	return false;
}

// projectile -> enemy collision detection
bool Util::CollisionDetection(const sf::CircleShape& projHitbox, const sf::CircleShape& enemyHitbox) {
	// calculate direction vector from the projectile's hitbox center to the enemy's hitbox center
	sf::Vector2f direction = enemyHitbox.getPosition() - projHitbox.getPosition();
	// calculate magnitude of the direction vector
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	// check if the distance is less than or equal to the sum of the radii
	float combinedRadius = projHitbox.getRadius() + enemyHitbox.getRadius();
	return distance <= combinedRadius; // return true if collision detected
}

// collision for projectiles and enemies, same concept as PlayerCollision() but slightly edited
bool Util::ProjectileCollision(const sf::CircleShape& projHitbox, const sf::CircleShape& enemyHitbox) {
	sf::Vector2f direction;
	if (Util::CollisionDetection(projHitbox, enemyHitbox)) {
		direction = enemyHitbox.getPosition() - projHitbox.getPosition();
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		direction = Util::NormalizeVector(direction);
		float collisionBoundary = 64.0f;
		sf::Vector2f boundaryPosition = enemyHitbox.getPosition() - direction * collisionBoundary;
		if (distance < collisionBoundary) {
		}
		return true;
	}
	return false;
}

// function to calculate the rotation based on the normalizedVector returned by the normalizeVector() function
float Util::CalculateRotation(sf::Vector2f vector) {
	// use atan2 instead of atan because atan assumes that both x and y are of the same sign (positive or negative) which means the angle will be limited to a 180� range
	// atan2 accounts for the normalized vectors x and y's individual signs meaning the angle can cover a full 360� and wont be limited to either 2 positive or 2 negative quadrants
	float radians = atan2(vector.y, vector.x);
	// get rotation degrees by multiplying the radians by 180 and then dividing by pi
	float degrees = radians * (180.0f / 3.14159265359f);
	return degrees;
}
