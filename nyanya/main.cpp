#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "game.h"
#include "player.h"

int main() {

		// -------------------------------------------------------------------------- INIT --------------------------------------------------------------------------
	
	// settings object
	sf::ContextSettings settings;
	// antialiasing level, can be 2, 4, or 8
	settings.antialiasingLevel = 8;
	// create sfml window
	sf::RenderWindow window(sf::VideoMode(1200, 700), "Game Window", sf::Style::Resize);
	// repeats button press if the button is held (inside the window)
	window.setKeyRepeatEnabled(false);

	// shape testing
	// size and points of circle, 4 points = a square etc.
	sf::CircleShape shape(50.0, 50);
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(sf::Vector2f(100, 100));
	shape.setOutlineThickness(10);
	shape.setOutlineColor(sf::Color::Magenta);
	
	// width and height of rectangle
	sf::RectangleShape rectangle(sf::Vector2f(200, 2));
	// position based on pixel point in screen
	rectangle.setPosition(sf::Vector2f(300, 100));
	rectangle.setFillColor(sf::Color::Blue);
	// pivot point of rectangle, currently set to the center because the getSize() = width value and height value, (width / 2.0 = half of width, height / 2.0 = half of height)
	rectangle.setOrigin(rectangle.getSize() / 2.0f);
	// angle changed by degrees based off of pivot point
	rectangle.setRotation(45);
	

	

		// -------------------------------------------------------------------------- INIT --------------------------------------------------------------------------

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


		// ------------------------------------------------------------------------- UPDATE -------------------------------------------------------------------------

		// -------------------------------------------------------------------------- DRAW --------------------------------------------------------------------------


		// clears window/screen each frame
		window.clear(sf::Color::Red);






		// copy frame from back buffer to window/screen
		window.display();


		// -------------------------------------------------------------------------- DRAW --------------------------------------------------------------------------
	}
	




}