#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "enemy.h"
#include "util.h"
#include "projectile.h"

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

	// initialize player object
	Player player;
	// call player.cpp function that loads player info (sprites, textures, position in window etc.)
	player.Load();

	Projectile projectile;
	projectile.Load();

	Enemy enemy;
	enemy.Load();


		// -------------------------------------------------------------------------- LOAD --------------------------------------------------------------------------

	// game loop
	while (window.isOpen()) {

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

		// call Update function through player object
		player.Update();
		enemy.Update();
		projectile.Update(window, player);

		// ------------------------------------------------------------------------- UPDATE -------------------------------------------------------------------------

		// -------------------------------------------------------------------------- DRAW --------------------------------------------------------------------------

		// clears window/screen each frame
		window.clear(sf::Color::Black);

		// calling draw functions to literally draw the sprite or shape to the render window
		player.Draw(window);
		enemy.Draw(window);
		projectile.Draw(window);

		// copy frame from back buffer to window/screen
		window.display();

		// -------------------------------------------------------------------------- DRAW --------------------------------------------------------------------------
	}
}