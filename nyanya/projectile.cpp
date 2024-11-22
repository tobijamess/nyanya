#include "projectile.h"
#include "util.h"

void Projectile::Initialize() {
	// initialize projectile size (size of image in pixels)
	projSize = sf::Vector2i(554, 322);	
	// initialize/set features for the hitbox(hitbox)
	projData.hitbox.setFillColor(sf::Color::Transparent);
	projData.hitbox.setOutlineColor(sf::Color::Red);
	projData.hitbox.setOutlineThickness(1);
	// projectile hitbox radius set to the scaled down width / 2 so the diameter matches the size of the sprite (322 x 0.1) / 2 = 16.7
	projData.hitbox.setRadius(16.7f);
	projData.hitbox.setOrigin(27.7f, 16.7f);
	
}

void Projectile::Load(Player& player) {
	// create vector2f projectileDirection object so it is loaded outside of the game loop (so it doesn't reset every single frame)
	sf::Vector2f projectileDirection;
	if (texture.loadFromFile("assets/player/projectiles/fireball.png")) {
		std::cout << "projectile image loaded\n";
		sprite.setTexture(texture);
		// scaleFactor vector scales down the sprite to * 0.1 of its original size
		sf::Vector2f scaleFactor = sf::Vector2f(0.1f, 0.1f);
		// set the actual sprite scale to scaleFactor vector
		sprite.scale(scaleFactor);
		// create spriteSize vector object to store the visible sprite size to be able to find the center
		sf::Vector2f spriteSize = sf::Vector2f(projSize.x * scaleFactor.x, projSize.y * scaleFactor.y);
		// find center by getting half of the images x pixels and y pixels and set sprite origin to center of that image
		sprite.setOrigin(spriteSize.x / 2, spriteSize.y / 2);
	}
}

void Projectile::Update(sf::RenderWindow& window, Player& player, float deltaTime) {
	// if left mouse button is pressed, execute following code
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		// create a new sprite variable (newProjectile)
		sf::Sprite newProjectile = sprite;
		// set the newProjectile variable to the players sprite positon
		newProjectile.setPosition(player.sprite.getPosition());

		// create new rectangle 'newHitbox' for each individual projectile stored in the struct
		sf::CircleShape newHitbox = projData.hitbox;
		// set the newHitbox variable to the players sprite positon
		newHitbox.setPosition(player.sprite.getPosition());
		
		// create temp vector2f mousePosition object for vector2i -> vector2f conversion
		// use static_cast to forcefully convert the vector2i's data (current mouse position) into the temporary mousePosition vector2f object
		sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

		// create new vector2f object (initialDirection) to store the direction of the projectile based on the result of the equation: target (mousePosition) - current (player.sprite.getPosition())
		sf::Vector2f initialDirection = mousePosition - player.sprite.getPosition();
		// create util object for normalize vector function
		Util util;
		// use util.cpp normalizeVector() function to normalize the vector2f that stores the (target - current) result
		initialDirection = util.normalizeVector(initialDirection);
		// use util.cpp calculateRotation() function to get the rotation angle from the normalized direction vector
		float rotation = Util::calculateRotation(initialDirection);
		// set the sprites rotation to the calculateRotation() functions result
		sprite.setRotation(rotation);
		// push newProjectile (rectangle variable) and its' intialDirection into the vector of structs
		playerProjectiles.push_back({ newHitbox, newProjectile, initialDirection });
	}
	// updates the position of each individual projectile based on it's direction stored in the playerProjectiles
	for (size_t i = 0; i < playerProjectiles.size(); i++)
	{
		// set position of the most recently added projectile to it's own stored position + it's direction and multiply it by speed and deltaTime to move it
		playerProjectiles[i].projectile.setPosition(
			playerProjectiles[i].projectile.getPosition() +
			playerProjectiles[i].direction * projectileSpeed * deltaTime);
		// set each projectile elements' hitbox to their sprites' position
		playerProjectiles[i].hitbox.setPosition(playerProjectiles[i].projectile.getPosition());
	}
	
}

void Projectile::Draw(sf::RenderWindow& window) {
	// for loop to draw each new projectile that gets added to the list during the Update() function 
	for (size_t i = 0; i < playerProjectiles.size(); i++)
	{
		// draws most recently added playerProjectiles element
		window.draw(playerProjectiles[i].projectile);
		window.draw(playerProjectiles[i].hitbox);
	}
}