#include "player.h"
#include "util.h"

// function to load player stuff like sprites, set default sprite position for movement etc.
// pass PlayerClass object as parameter to access PlayerClass members and change them
void playerLoad(PlayerClass& player) {

	// load texture from file into tempPlayerTexture object
	if (player.playerTexture.loadFromFile("assets/player/textures/playerspritesheet.png")) {
		// successful load
		std::cout << "player image loaded\n";
		// set tempPlayerTexture object equal to
		// use setTexture function to set the sprite to playerTexture (which holds the .png)
		player.playerSprite.setTexture(player.playerTexture);

		// function to put rectangle (sf::IntRect) over the pixels you want to select for drawing within the texture by setting x, y and width/height (all integers)
		// xIndex is sprite position in the row, yIndex is sprite position in column, is multiplied by 64 so the rectangle can move over/down 1 sprite at a time
		// all player sprites are 64x64 so rectWidth and rectHeight stay at 64, 64
		player.playerSprite.setTextureRect(sf::IntRect(player.xIndex * 64, player.yIndex * 64, 64, 64));
		// multiplies the current scale of playerSprite object (make sprite bigger)
		player.playerSprite.scale(sf::Vector2f(3, 3));
		player.playerSprite.setPosition(sf::Vector2f(800, 500));
	}
	else {
		// failed load
		std::cout << "player image didnt load\n";
	}
	return;
}

// function to take player input and change position of player sprite based on input
// pass PlayerClass object as parameter to access PlayerClass members and change them
void playerMove(PlayerClass& player) {

	// create vector2f object ('position', which takes x, y) to store the value (x, y) of playerSprite.getPosition which is by default 0,0 (top left of window)
	sf::Vector2f position = player.playerSprite.getPosition();

	// read the realtime keyboard, check if a key was pressed, if the key pressed was 'W', run the code
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player.playerSprite.setPosition(position + sf::Vector2f(0.00f, -0.1f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player.playerSprite.setPosition(position + sf::Vector2f(-0.1f, 0.00f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player.playerSprite.setPosition(position + sf::Vector2f(0.00f, 0.1f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { 
		player.playerSprite.setPosition(position + sf::Vector2f(0.1f, 0.00f));
	}
	// same as above but combining mulitple movement keys pressed
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) + sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player.playerSprite.setPosition(position + sf::Vector2f(-0.05f, -0.05f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) + sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.playerSprite.setPosition(position + sf::Vector2f(0.05f, -0.05f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) + sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player.playerSprite.setPosition(position + sf::Vector2f(-0.05f, 0.05f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) + sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.playerSprite.setPosition(position + sf::Vector2f(0.05f, 0.05f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) + sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player.playerSprite.setPosition(position + sf::Vector2f(0.0f, 0.0f));
	}*/
}

// function to draw player sprite to the window
// pass PlayerClass & RenderWindow class object's as parameters so draw function can draw the playerTexture to the render window
void playerDraw(PlayerClass& player, sf::RenderWindow& window) {
	// draw the sprite to window, only sprites can be drawn not textures
	window.draw(player.playerSprite);
}

void playerProjectileLoad(PlayerClass& player){
	// create stdlib vector to store list of player projectiles
	std::vector<sf::RectangleShape> playerProjectiles;
	// set projectile speed
	float projectileSpeed = 0.5f;
	// create vector2f projectileDirection object so it is loaded outside of the game loop (so it doesn't reset every single frame)
	sf::Vector2f projectileDirection;
}

void playerProjectileMove(PlayerClass& player, sf::RenderWindow& window) {
	// if left mouse button is pressed, execute following code
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		// create a new rectangle variable (newProjectile) and then push it back (add it) into the list (stdlib vector) of projectile elements
		sf::RectangleShape newProjectile(sf::Vector2f(50, 25));
		player.playerProjectiles.push_back(newProjectile);
		// get the most recently added element from the projectile list by finding the size (number of elements in the list) and subtracting 1 (because the index begins at 0 not 1)
		// then set the position of that last element from the projectile list to the players' sprite position so it always comes from the player
		int i = player.playerProjectiles.size() - 1;
		player.playerProjectiles[i].setPosition(player.playerSprite.getPosition());
	}

	// while the size of the playerProjectiles list is greater than 0 (i = 0), run the loop and increment i
	for (size_t i = 0; i < player.playerProjectiles.size(); i++)
	{
		// create temp vector2f mousePosition object for vector2i's data -> vector2f conversion
		sf::Vector2f mousePosition;
		// use static_cast to forcefully convert the vector2i's data (current mouse position) into the temporary mousePosition vector2f object
		mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
		// create new vector2f object (projectileDirection) to store the direction of the projectile based on the result of the equation: target (mousePosition) - current (playerProjectiles[i].getPosition())
		sf::Vector2f projectileDirection = mousePosition - player.playerProjectiles[i].getPosition();
		// use util.cpp normalizeVector() function to normalize the vector2f that stores the direction equation result
		projectileDirection = normalizeVector(projectileDirection);
		// access each individual element with playerProjectiles[i] in which new elements are being created every time LMB is pressed
		// then set those elements' position to their own position (because their position is set to the playerSprites' position every time a new element is created)
		// lastly, adds the direction that the projectile needs to move in to its' original position (on the playerSprite) and then multiply it by the projectile speed
		player.playerProjectiles[i].setPosition(player.playerProjectiles[i].getPosition() + player.projectileDirection * player.projectileSpeed);
	}
}

void playerProjectileDraw(PlayerClass& player, sf::RenderWindow& window) {
	// for loop to draw each new projectile that gets added to the list during the playerProjectileMove() function 
	// because each time LMB is pressed, a newProjectile element is pushed back into the playerProjectiles list (stdlib vector)
	for (size_t i = 0; i < player.playerProjectiles.size(); i++)
	{
		// draws most recent playerProjectiles element
		window.draw(player.playerProjectiles[i]);
	}
}