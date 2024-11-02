#include <iostream>
#include "player.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

//func to intialize player
int initPlayer() {

	// create player object and use it to read and edit class members' values
	PlayerClass player;
	// initialize player setting its value to true
	player.playerInit = true;

	return 0;
	

}

void playerInput() {

	// create player object and use it to read and edit class members' values
	PlayerClass player;

	// playerInit was set to true in initPlayer(), so now take user inputs for game loop
	if (player.playerInit = true) {
		std::cout << "test";
	}

	// using sfml to get keyboard input and create movement functions based on players input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		//player.playerMove(-1.f, 0.f);
	}




}