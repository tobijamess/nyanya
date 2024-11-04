#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "game.h"
#include "player.h"
#include "enemy.h"
#include "util.h"


	

int main() {

		// -------------------------------------------------------------------------- INIT --------------------------------------------------------------------------
	
	// settings object
	sf::ContextSettings settings;
	// antialiasing level, can be 2, 4, or 8
	settings.antialiasingLevel = 8;
	// create sfml window
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game Window", sf::Style::Default);
	// repeats button press if the button is held (inside the window)
	window.setKeyRepeatEnabled(false);


		// -------------------------------------------------------------------------- INIT --------------------------------------------------------------------------

	



	
		// -------------------------------------------------------------------------- LOAD --------------------------------------------------------------------------


	// initialize player object for use in player.cpp's function parameters
	PlayerClass player;
	// initialize enemy object for use in enemy.cpp's function parameters
	EnemyClass enemy;

	// call player.cpp function that loads player info (sprites, textures, position in window etc.)
	playerLoad(player);
	// call enemy.cpp function that loads enemy info (sprites, textures, position in window etc.)
	enemyLoad(enemy);
	
	playerProjectileLoad(player);

		// -------------------------------------------------------------------------- LOAD --------------------------------------------------------------------------

	// game loop
	while (window.isOpen()) {

		/*bullet.setPosition(bullet.getPosition() + bulletDirection * bulletSpeed);*/

		// ------------------------------------------------------------------------- UPDATE -------------------------------------------------------------------------


		// 'event' object of type 'Event' class
		sf::Event event;
		// pollEvent() function checks if there is an event and stores it in 'event' object
		while (window.pollEvent(event)) {

			// check the event type stored in 'event' object, if event type is equal to the closed event, then close the window
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// call playerInput function with player object as parameter
		playerMove(player);
		enemyMove(enemy);
		playerProjectileMove(player, window);
		


		// ------------------------------------------------------------------------- UPDATE -------------------------------------------------------------------------

		// -------------------------------------------------------------------------- DRAW --------------------------------------------------------------------------


		// clears window/screen each frame
		window.clear(sf::Color::Black);



		// calling draw functions to literally draw the sprite or shape to the render window
		playerDraw(player, window);
		enemyDraw(enemy, window);
		playerProjectileDraw(player, window);



		// copy frame from back buffer to window/screen
		window.display();


		// -------------------------------------------------------------------------- DRAW --------------------------------------------------------------------------
	}
}




//// shape testing
//// size and points of circle, 4 points = a square etc.
//sf::CircleShape shape(50.0, 50);
//shape.setFillColor(sf::Color::Red);
//shape.setPosition(sf::Vector2f(100, 100));
//shape.setOutlineThickness(10);
//shape.setOutlineColor(sf::Color::Magenta);
//
//// width and height of rectangle
//sf::RectangleShape rectangle(sf::Vector2f(200, 2));
//// position based on pixel point in screen
//rectangle.setPosition(sf::Vector2f(300, 100));
//rectangle.setFillColor(sf::Color::Blue);
//// pivot point of rectangle, currently set to the center because the getSize() = width value and height value, (width / 2.0 = half of width, height / 2.0 = half of height)
//rectangle.setOrigin(rectangle.getSize() / 2.0f);
//// angle changed by degrees based off of pivot point
//rectangle.setRotation(45);