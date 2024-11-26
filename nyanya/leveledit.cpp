#include "leveledit.h"
#include "game.h"

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
				tile.setOrigin(sf::Vector2f(0, 0));
				// push new tile back into vector of tiles (tileOptions)
				tileOptions.push_back(tile);
			}
		}
	}
	else {
		std::cout << "tilemap didnt load\n";
	}
	std::cout << "tile options loaded: " << tileOptions.size() << "\n";
}

void LevelEdit::CreateTile(const sf::Vector2f& position) {
	int x = position.x / 64;
	int y = position.y / 64;
	// set sprite object 'tile' to current tile in the index
	sf::Sprite tile = tileOptions[tileOptionIndex];
	tile.setPosition(x * 64, y * 64);
	// iterate throguh tiles vector (list)
	for (int i = 0; i < tiles.size(); ++i) {
		// check if there is already a tile at this position, if there is erase it
		if (tiles[i].getPosition() == tile.getPosition()) {
			// make i'th tile equivalent to the tile at the back (set it to the back of the list)
			tiles[i] = tiles.back();
			// erase the i'th tile which is now at the back of the list
			tiles.pop_back();
			break;
		}
	}
	tiles.push_back(tile);
}

void LevelEdit::RemoveTile(const sf::Vector2f& position) {
	int x = position.x / 64;
	int y = position.y / 64;
	sf::Vector2f tilePosition(x * 64, y * 64);
	// iterate throguh tiles vector (list)
	for (int i = 0; i < tiles.size(); ++i) {
		// check if there is already a tile at this position, if there is erase it
		if (tiles[i].getPosition() == tilePosition) {
			// make i'th tile equivalent to the tile at the back (set it to the back of the list)
			tiles[i] = tiles.back();
			// erase the i'th tile which is now at the back of the list
			tiles.pop_back();
			break;
		}
	}
}

void LevelEdit::Update(sf::RenderWindow& window, Game& game) {
	// do nothing if not in level editor game state
	if (game.getGameMode() != Game::LevelEditor) {
		return;
	}
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

void LevelEdit::Draw(sf::RenderWindow& window, Game& game) {
	// loop to validate size of tileOptions vector before setting values like positions
	if (game.getGameMode() == Game::LevelEditor && !tileOptions.empty() && tileOptionIndex >= 0 && tileOptionIndex < tileOptions.size()) {
		// get mouse pos within game window
		sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
		// set position of selected tile (tileoption index) to the mouse position
		tileOptions[tileOptionIndex].setPosition(mousePosition);
		// draw that selected tile
		window.draw(tileOptions[tileOptionIndex]);
	}
	// iterate through each tile within the tiles vector of sprites and draw to application window
	for (const sf::Sprite& tile : tiles) {
		window.draw(tile);
	}
}