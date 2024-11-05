#include "projectile.h"
#include "util.h"

void Projectile::Initialize() {

}

void Projectile::Load() {
	// create vector2f projectileDirection object so it is loaded outside of the game loop (so it doesn't reset every single frame)
	sf::Vector2f projectileDirection;
}

void Projectile::Update(sf::RenderWindow& window, Player& player, float deltaTime) {
	// if left mouse button is pressed, execute following code
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

		// create a new rectangle variable (newProjectile)
		sf::RectangleShape newProjectile(sf::Vector2f(50, 25));
		// set the newProjectile variable to the players sprite positon
		newProjectile.setPosition(player.sprite.getPosition());
		
		// create temp vector2f mousePosition object for vector2i -> vector2f conversion
		// use static_cast to forcefully convert the vector2i's data (current mouse position) into the temporary mousePosition vector2f object
		sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

		// create new vector2f object (initialDirection) to store the direction of the projectile based on the result of the equation: target (mousePosition) - current (player.sprite.getPosition())
		sf::Vector2f initialDirection = mousePosition - player.sprite.getPosition();
		// create util object for normalize vector function
		Util util;
		// use util.cpp normalizeVector() function to normalize the vector2f that stores the (target - current) result
		initialDirection = util.normalizeVector(initialDirection);
		// push newProjectile (rectangle variable) and its' intialDirection into the vector of structs
		playerProjectiles.push_back({ newProjectile, initialDirection });
	}
	// updates the position of each individual projectile based on it's direction stored in the playerProjectiles
	for (size_t i = 0; i < playerProjectiles.size(); i++)
	{
		// set position of the most recently added projectile to it's own stored position + it's direction and multiply it by speed and deltaTime to move it
		playerProjectiles[i].projectile.setPosition(
			playerProjectiles[i].projectile.getPosition() + 
			playerProjectiles[i].direction * projectileSpeed * deltaTime);
	}
}

void Projectile::Draw(sf::RenderWindow& window) {
	// for loop to draw each new projectile that gets added to the list during the Update() function 
	for (size_t i = 0; i < playerProjectiles.size(); i++)
	{
		// draws most recent playerProjectiles element
		window.draw(playerProjectiles[i].projectile);
	}
}