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
	// settings object
	sf::ContextSettings settings;
	// antialiasing level, can be 2, 4, or 8
	settings.antialiasingLevel = 8;
	// create sfml window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Game Window", sf::Style::Default);
	// repeats button press if the button is held (inside the window)
	window.setKeyRepeatEnabled(false);


	// initialize player object
	Player player;
	Projectile projectile;
	Enemy enemy;



	// initalize sprite size and hitbox
	player.Initialize();
	// call player.cpp function that loads player info (sprites, textures, position in window etc.)
	player.Load();

	projectile.Initialize();
	projectile.Load(player);

	enemy.Initialize();
	enemy.Load();



	// clock object initalization to use for getting the time each frame takes to render
	sf::Clock clock;

	// game loop
	while (window.isOpen()) {

		// restart the timer at the beginning of every window frame
		sf::Time timer = clock.restart();
		// display time for each frame in console in milliseconds
		//std::cout << timer.asSeconds() << std::endl;
		// store the timer value in deltaTime variable for use in other functions (player.Update() etc.)
		float deltaTime = timer.asSeconds();

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
		player.Update(player, enemy, deltaTime);
		enemy.Update(deltaTime);
		projectile.Update(window, player, enemy, deltaTime);



		// clears window/screen each frame
		window.clear(sf::Color::Black);

		// calling draw functions to literally draw the sprite or shape to the render window
		player.Draw(window);
		enemy.Draw(window);
		projectile.Draw(window);

		// copy frame from back buffer to window/screen
		window.display();
	}
}