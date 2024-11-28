#include "leveledit.h"
#include "game.h"
#include "tilemap.h"

void LevelEdit::Initialize(Game& game) {
	// set scrollWheelInput to None by default
	game.scrollWheelInput = Game::ScrollWheel::None;
	tileOptionIndex = 0;
	// initialize the view to window size, position and center
	view.setSize(game.window.getSize().x, game.window.getSize().y);
	view.setCenter(view.getSize().x / 2.0f, view.getSize().y / 2.0f);
}

void LevelEdit::Load() {
	if (texture.loadFromFile("assets/map/tilemap.png")) {
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
	std::cout << "tile options loaded: " << tileOptions.size() << "\n";
	// for keybind overlay
	if (font.loadFromFile("assets/fonts/font.ttf")) {
		keybindText.setFont(font);
		keybindText.setCharacterSize(20);
		keybindText.setFillColor(sf::Color::White);
		keybindText.setString(
			"Keybinds:\n"
			"1: Switch to Layer 1\n"
			"2: Switch to Layer 2\n"
			"3: Switch to Layer 3\n"
			"4: Switch to Collision Layer\n"
			"H: Toggle Keybind Overlay\n"
			"Left Click: Place Tile\n"
			"Right Click: Remove Tile\n"
			"Scroll: Change Tile\n"
			"Middle Click: Drag Map\n"
		);
		keybindText.setPosition(10.0f, 10.0f);
	}
}

// function to toggle keybind overlay on and off
void LevelEdit::ToggleKeybindOverlay() { 
	showKeybindOverlay = !showKeybindOverlay; 
}

void LevelEdit::Update(sf::RenderWindow& window, Game& game, TileMap& tilemap) {
	// do nothing if not in level editor game state
	if (game.GetGameMode() != Game::LevelEditor) return;
	// get mouse pos in world coords for use in create tile and remove tile
	worldMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	// if mmb is pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		// if not dragging, start dragging
		if (!isDragging) {
			isDragging = true;
			// save initial mouse pos in world coords
			initialMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
		}
		else {
			// calculate movement delta in world coords and adjust the view based on that
			sf::Vector2f currentMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
			sf::Vector2f delta = initialMousePos - currentMousePos;
			// move the view by the delta
			view.move(delta);
			window.setView(view);
		}
	}
	else {
		// reset dragging when mmb is released
		isDragging = false;
	} 
	static bool wasHPressed = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
		if (!wasHPressed) {
			ToggleKeybindOverlay();
			wasHPressed = true;
		}
	} 
	else {
		wasHPressed = false;
	}
	// when left mouse button is pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// call create tile function with mouse position passed as param
		CreateTile(worldMousePos, game.GetTileMap());
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		RemoveTile(worldMousePos, game.GetTileMap());
	}
	// tile preview image: if the scrollwheelinput object is set to enum member ScrollUp
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
		game.scrollWheelInput = Game::ScrollWheel::None;
	}
	// update the tile preview to follow the mouse no matter where in the grid (now using worldmousepos instead of mousepos on screen)
	if (!tileOptions.empty() && tileOptionIndex >= 0 && tileOptionIndex < tileOptions.size()) {
		sf::Sprite& previewTile = tileOptions[tileOptionIndex];
		// use tilesize for all layers except for collision layer
		float gridSize = (tilemap.GetActiveLayerIndex() != 3) ? tilemap.tileSize : tilemap.tileSize / 2.0f;
		previewTile.setPosition(
			std::floor(worldMousePos.x / gridSize) * gridSize,
			std::floor(worldMousePos.y / gridSize) * gridSize
		);
		previewTile.setScale(tilemap.tileSize / 64.0f, tilemap.tileSize / 64.0f); // adjust the scale
	}
}

void LevelEdit::Draw(sf::RenderWindow& window, Game& game) {
	// loop to validate size of tileOptions vector before setting values like positions
	if (game.GetGameMode() == Game::LevelEditor) {
		// draw leveleditor view
		window.setView(view);
		// validate and draw selected tile preview
		if (!tileOptions.empty() && tileOptionIndex >= 0 && tileOptionIndex < static_cast<int>(tileOptions.size())) {
			// set the preview tile to be drawn based on whatever index we have scrolled to in tileOptions
			sf::Sprite& previewTile = tileOptions[tileOptionIndex];
			// set tile preview image to world coords mouse position
			previewTile.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window), view));
			window.draw(previewTile);
		}
	}
	// iterate through each tile within the tiles vector of sprites and draw to application window
	for (const sf::Sprite& tile : tiles) {
		window.draw(tile);
	}
	// draw keybind overlay without resetting the view
	if (showKeybindOverlay) {
		// use default view for keybind overlay to keep it fixed in screen space
		sf::View originalView = window.getView();
		window.setView(window.getDefaultView());
		window.draw(keybindText);
		window.setView(originalView);
	}
}


void LevelEdit::CreateTile(const sf::Vector2f& position, TileMap& tilemap) {
	int x = position.x / tilemap.tileSize;
	int y = position.y / tilemap.tileSize;
	if (tilemap.GetActiveLayerIndex() == 3) {
		// use finer grid for collision map
		tilemap.SetCollision(x, y, 2);
	}
	else {
		// store using default grid size
		tilemap.SetTile(x, y, tileOptionIndex);
	}
}

void LevelEdit::RemoveTile(const sf::Vector2f& position, TileMap& tilemap) {
	int x = position.x / tilemap.tileSize;
	int y = position.y / tilemap.tileSize;
	if (tilemap.GetActiveLayerIndex() == 3) {
		tilemap.SetCollision(x, y, 1);
	}
	else {
		int x = position.x / tilemap.tileSize;
		int y = position.y / tilemap.tileSize;
		tilemap.SetTile(x, y, -1);
	}
}