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
			"M: Toggle Merged Layer View\n"
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
	// toggle the keybind overlay when pressing H
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
	// toggle the merge layer view when pressing M
	static bool wasMPressed = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
		if (!wasMPressed) {
			showMergedLayers = !showMergedLayers;
			wasMPressed = true;
		}
	}
	else {
		wasMPressed = false;
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
	if (tilemap.GetActiveLayerIndex() != 3 && !tileOptions.empty() && tileOptionIndex >= 0 && tileOptionIndex < tileOptions.size()) {
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

void LevelEdit::Draw(sf::RenderWindow& window, Game& game, TileMap& tilemap) {
	if (game.GetGameMode() == Game::LevelEditor) {
		window.setView(view);

		// Draw non-collision layers (Layer 1, 2, 3)
		for (int layerIndex = 0; layerIndex < 3; ++layerIndex) {
			const auto& layer = game.GetTileMap().GetTileMapLayer(layerIndex);
			for (int y = 0; y < layer.size(); ++y) {
				for (int x = 0; x < layer[y].size(); ++x) {
					int tileID = layer[y][x];
					if (tileID >= 0 && tileID < tileOptions.size()) {
						sf::Sprite& tile = tileOptions[tileID];
						tile.setPosition(x * tilemap.tileSize, y * tilemap.tileSize);
						tile.setScale(tilemap.tileSize / 64.0f, tilemap.tileSize / 64.0f);

						// Adjust transparency for merge layer view
						if (showMergedLayers && layerIndex != game.GetTileMap().GetActiveLayerIndex()) {
							tile.setColor(sf::Color(255, 255, 255, 150)); // Semi-transparent
						}
						else {
							tile.setColor(sf::Color(255, 255, 255, 255)); // Fully opaque
						}
						window.draw(tile);
					}
				}
			}
		}

		// draw the collision layer above all other layers
		const auto& collisionLayer = game.GetTileMap().GetTileMapLayer(3);
		for (int y = 0; y < collisionLayer.size(); ++y) {
			for (int x = 0; x < collisionLayer[y].size(); ++x) {
				if (collisionLayer[y][x] == 2) {
					sf::RectangleShape collisionTile(sf::Vector2f(tilemap.tileSize / 2.0f, tilemap.tileSize / 2.0f));
					collisionTile.setPosition(x * (tilemap.tileSize / 2.0f), y * (tilemap.tileSize / 2.0f));

					// dynamic transparency for the collision layer
					if (game.GetTileMap().GetActiveLayerIndex() == 3) {
						collisionTile.setFillColor(sf::Color(255, 0, 0, 255)); // Fully opaque
					}
					else if (showMergedLayers) {
						collisionTile.setFillColor(sf::Color(255, 0, 0, 150)); // Semi-transparent
					}
					else {
						collisionTile.setFillColor(sf::Color(255, 0, 0, 150)); // Semi-transparent (default)
					}

					window.draw(collisionTile);
				}
			}
		}

		// Render preview options
		if (!tileOptions.empty() && tileOptionIndex >= 0 && tileOptionIndex < tileOptions.size()) {
			sf::Sprite& previewTile = tileOptions[tileOptionIndex];
			previewTile.setPosition(worldMousePos.x, worldMousePos.y);
			previewTile.setColor(sf::Color(255, 255, 255, 255)); // Fully opaque
			previewTile.setScale(tilemap.tileSize / 64.0f, tilemap.tileSize / 64.0f);
			window.draw(previewTile);
		}

		if (showKeybindOverlay) {
			window.draw(keybindText);
		}
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