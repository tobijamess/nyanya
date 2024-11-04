#include "projectile.h"
#include "util.h"

void Projectile::Initialize() {

}

void Projectile::Load() {
	// create stdlib vector to store list of player projectiles
	std::vector<sf::RectangleShape> playerProjectiles;
	// set projectile speed
	float projectileSpeed = 0.6f;
	// create vector2f projectileDirection object so it is loaded outside of the game loop (so it doesn't reset every single frame)
	sf::Vector2f projectileDirection;
}

void Projectile::Update(sf::RenderWindow& window, Player& player) {
	// if left mouse button is pressed, execute following code
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		// create a new rectangle variable (newProjectile) and then push it back (add it) into the list (stdlib vector) of projectile elements
		sf::RectangleShape newProjectile(sf::Vector2f(50, 25));
		playerProjectiles.push_back(newProjectile);
		// get the most recently added element from the projectile list by finding the size (number of elements in the list) and subtracting 1 (because the index begins at 0 not 1)
		// then set the position of that last element from the projectile list to the players' sprite position so it always comes from the player
		int i = playerProjectiles.size() - 1;
		playerProjectiles[i].setPosition(player.sprite.getPosition());
	}

	// while the size of the playerProjectiles list is greater than 0 (i = 0), run the loop and increment i
	for (size_t i = 0; i < playerProjectiles.size(); i++)
	{
		// create temp vector2f mousePosition object for vector2i's data -> vector2f conversion
		sf::Vector2f mousePosition;
		// use static_cast to forcefully convert the vector2i's data (current mouse position) into the temporary mousePosition vector2f object
		mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
		// create new vector2f object (projectileDirection) to store the direction of the projectile based on the result of the equation: target (mousePosition) - current (playerProjectiles[i].getPosition())
		sf::Vector2f projectileDirection = mousePosition - playerProjectiles[i].getPosition();
		// use util.cpp normalizeVector() function to normalize the vector2f that stores the direction equation result
		projectileDirection = normalizeVector(projectileDirection);
		// access each individual element with playerProjectiles[i] in which new elements are being created every time LMB is pressed
		// then set those elements' position to their own position (because their position is set to the playerSprites' position every time a new element is created)
		// lastly, adds the direction that the projectile needs to move in to its' original position (on the playerSprite) and then multiply it by the projectile speed
		playerProjectiles[i].setPosition(playerProjectiles[i].getPosition() + projectileDirection * projectileSpeed);
	}
}

void Projectile::Draw(sf::RenderWindow& window) {
	// for loop to draw each new projectile that gets added to the list during the playerProjectileMove() function 
	// because each time LMB is pressed, a newProjectile element is pushed back into the playerProjectiles list (stdlib vector)
	for (size_t i = 0; i < playerProjectiles.size(); i++)
	{
		// draws most recent playerProjectiles element
		window.draw(playerProjectiles[i]);
	}
}