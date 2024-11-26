#include "leveledit.h"
#include "game.h"

// constructor with init list
//LevelEdit::LevelEdit() {
//
//}

void LevelEdit::Initialize(Game& game) {
	// set scrollWheelInput to None by default
	game.scrollWheelInput = Game::ScrollWheel::None;
	tileOptionIndex = 0;
}

void LevelEdit::Load() {
	if (texture.loadFromFile("assets/map/tilemap.jpg")) {
		std::cout << "tilemap loaded successfully\n";
		// iterate down columns of sprites in the tilemap.png second
		for (int y = 0; y < 4; ++y) {
			// iterate along rows of sprites in the tilemap.png first
			for (int x = 0; x < 4; ++x) {
				sf::Sprite tile;
				tile.setTexture(texture);
				// first two values are position on tilemap.png, second two values are width and height of rect to get
				tile.setTextureRect(sf::IntRect(x * 64, y * 64, 64, 64));
				tile.setScale(sf::Vector2f(1.0f, 1.0f));
				tile.setOrigin(sf::Vector2f(32, 32));
				// push new tile back into vector of tiles (tileOptions)
				tileOptions.push_back(tile);
			}
		}
	}
	else {
		std::cout << "tilemap didnt load\n";
	}
	std::cout << "Tile options loaded: " << tileOptions.size() << std::endl;
}

void LevelEdit::Update(sf::RenderWindow& window, Game& game) {
	// when left mouse button is pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// get mouse position within the window
		sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
		// call create tile function with mouse position passed as param
		CreateTile(mousePosition);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
		RemoveTile(mousePosition);
	}
	// if the scrollwheelinput object is set to enum member ScrollUp
	if (game.scrollWheelInput == Game::ScrollWheel::ScrollUp) {
		// increment the tile input index
		tileOptionIndex++;
		// if tile input index reaches the end (>=), then just go back to beginning (tileOptionIndex = 0)
		if (tileOptionIndex >= tileOptions.size()) {
			tileOptionIndex = 0;
		}
		// reset
		game.scrollWheelInput = Game::ScrollWheel::None;
	}
	// same for scroll down but opposite
	else if (game.scrollWheelInput == Game::ScrollWheel::ScrollDown) {
		tileOptionIndex--;
		if (tileOptionIndex < 0) {
			tileOptionIndex = tileOptions.size() - 1;
		}
		// reset
		game.scrollWheelInput = Game::ScrollWheel::None;
	}
}

void LevelEdit::CreateTile(const sf::Vector2f& position) {

}

void LevelEdit::RemoveTile(const sf::Vector2f& position) {

}

void LevelEdit::Draw(sf::RenderWindow& window) {
	// loop to validate size of tileOptions vector before setting values like positions
	if (!tileOptions.empty() && tileOptionIndex >= 0 && tileOptionIndex < tileOptions.size()) {
		// get mouse pos within game window
		sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
		// set position of selected tile (tileoption index) to the mouse position
		tileOptions[tileOptionIndex].setPosition(mousePosition);
		// draw that selected tile
		window.draw(tileOptions[tileOptionIndex]);
	}
}