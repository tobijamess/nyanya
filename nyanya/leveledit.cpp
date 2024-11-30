#include "leveledit.h"
#include "game.h"
#include "tilemap.h"

void LevelEdit::Initialize(Game& game) {
	// set scrollWheelInput to None by default to make sure no initial scroll input is processed
	game.scrollWheelInput = Game::ScrollWheel::None;
	// set tileOptionIndex to 0 so it always displays the first tile sprite
	tileOptionIndex = 0;
	// configure the view based on the window dimensions and then set the views center to half of the dimensions so it always starts centered
	view.setSize(game.window.getSize().x, game.window.getSize().y);
	view.setCenter(view.getSize().x / 2.0f, view.getSize().y / 2.0f);
}

void LevelEdit::Load() {
	if (texture.loadFromFile("assets/map/tilemap64.png")) {
		std::cout << "tilemap loaded successfully\n";
		// iterate over a 3x3 grid to get individual tile sprites and store them in the tileOptions vector (change the grid values based on the spritesheet tile number)
		for (int y = 0; y < 3; ++y) {
			for (int x = 0; x < 5; ++x) {
				sf::Sprite tile;
				tile.setTexture(texture);
				// set the rectangle to put over the spritesheet and get the sprites based on the dimensions of the rect (first two values are position on tilesheet, second two values are width and height of the sprite itself)
				tile.setTextureRect(sf::IntRect(x * 64, y * 64, 64, 64));
				tile.setScale(sf::Vector2f(1.0f, 1.0f));
				tile.setOrigin(sf::Vector2f(0, 0));
				// push new tile back into vector of tile sprites (tileOptions)
				tileOptions.push_back(tile);
			}
		}
	}
	std::cout << "tile options loaded: " << tileOptions.size() << "\n";
	// load font and set properties for the keybind overlay used in the level editor
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

void LevelEdit::Update(sf::RenderWindow& window, Game& game, TileMap& tilemap) {
	// do nothing if not in level editor game state
	if (game.GetGameMode() != Game::LevelEditor) return;
	// get mouse pos in world coords for use in LevelEdit::CreateTile/RemoveTile
	worldMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		// if isDragging is false when middle mouse button is pressed, set it to true
		if (!isDragging) {
			isDragging = true;
			// save initial mouse pos in world coords
			initialMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
		}
		else {
			// calculate movement delta in world coords and adjust the view based on that
			sf::Vector2f currentMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
			sf::Vector2f delta = initialMousePos - currentMousePos;
			// move the view by the delta to give the dragging across the screen effect
			view.move(delta);
			window.setView(view);
		}
	}
	else {
		// reset dragging when middle mouse button is released
		isDragging = false;
	}

	// if the bool wasHPressed is still false when the 'H' key is pressed (meaning it hasn't already been pressed), invert showKeybindOverlay's value and wasHPressed to true (meaning for the next update frame, it has already been pressed)
	// since showKeybindOverlays default value is false, the inversion allows LevelEdit::Draw to check if showKeybindOverlay is now true to draw the keybindText
	static bool wasHPressed = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
		if (!wasHPressed) {
			showKeybindOverlay = !showKeybindOverlay;
			wasHPressed = true;
		}
	}
	else {
		wasHPressed = false;
	}

	// same functionality as wasHPressed but for toggling between the merged layer view
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

	// call create or remove tile functions dependant on which mouse button was pressed (left = create, right = remove)
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { CreateTile(worldMousePos, game.GetTileMap()); }
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) { RemoveTile(worldMousePos, game.GetTileMap()); }

	// Game::ProcessEvents updates the enum object scrollWheelInput based on which direction the scroll wheel was moved
	// if the scroll wheel was scrolled up, increment the tileOptionIndex int whos' value is relative to the tileOptions vector of sprites
	// but if the tileOptionIndex int is incremented to a point where it is larger than the amount of sprites in the tileOptions vector, it is reset to 0 (if you scroll to end of preview sprites, it resets to the beginning again)
	if (game.scrollWheelInput == Game::ScrollWheel::ScrollUp) {
		tileOptionIndex++;
		if (tileOptionIndex >= tileOptions.size()) {
			tileOptionIndex = 0;
		}
		// reset enum object scrollWheelInput back to None
		game.scrollWheelInput = Game::ScrollWheel::None;
	}
	// same as above but for scrolling down to decrement the tile option sprite preview
	else if (game.scrollWheelInput == Game::ScrollWheel::ScrollDown) {
		tileOptionIndex--;
		if (tileOptionIndex < 0) {
			tileOptionIndex = tileOptions.size() - 1;
		}
		game.scrollWheelInput = Game::ScrollWheel::None;
	}
	// if the active layers index is not 3, tileOptions vector isn't empty, tileOptionIndex int isn't 0, and tileOptionIndex int value is smaller than the number of sprites in tileOptions vector
	// basically: active layer isn't the collision map, tileOptions has sprites in it, tileOptionIndex has been incremented by scrolling then, set previewTile to the tileOption sprite corresponding with the tileOptionIndex value
	if (tilemap.GetActiveLayerIndex() != 3 && !tileOptions.empty() && tileOptionIndex >= 0 && tileOptionIndex < tileOptions.size()) {
		sf::Sprite& previewTile = tileOptions[tileOptionIndex];
		// set gridSize to the tilemap's tileSize, or tileSize / 2 based on if the active layer's index is or isn't 3 (collision maps' index)
		float gridSize = (tilemap.GetActiveLayerIndex() != 3) ? tilemap.tileSize : tilemap.tileSize / 2.0f;
		// set the position of the previewTile to the grid location where the mouse was
		previewTile.setPosition(
			std::floor(worldMousePos.x / gridSize) * gridSize,
			std::floor(worldMousePos.y / gridSize) * gridSize
		);
		previewTile.setScale(tilemap.tileSize / 32.0f, tilemap.tileSize / 32.0f); // adjust the scale
	}
}

void LevelEdit::Draw(sf::RenderWindow& window, Game& game, TileMap& tilemap) {
	window.setView(view);
	// use TileMap::Draw with parameters: window (render target), game (provide game mode context), *this (passes the LevelEdit object instance), showMergedLayers (whether layers are merged), and game... (specifies the active layer)
	game.GetTileMap().Draw(window, game, *this, showMergedLayers, game.GetTileMap().GetActiveLayerIndex());

	// the same tile preview functionality as in the LevelEdit::Update function above, but for drawing the tile option to the render window
	if (tilemap.GetActiveLayerIndex() != 3 && !tileOptions.empty() && tileOptionIndex >= 0 && tileOptionIndex < tileOptions.size()) {
		sf::Sprite& previewTile = tileOptions[tileOptionIndex];
		previewTile.setPosition(
			std::floor(worldMousePos.x / tilemap.tileSize) * tilemap.tileSize,
			std::floor(worldMousePos.y / tilemap.tileSize) * tilemap.tileSize
		);
		// scale the previewTile sprite to match the current tileSize
		previewTile.setScale(tilemap.tileSize / 32.0f, tilemap.tileSize / 32.0f);
		// slight transparency so you can differentiate the tile preview from the placed tiles in level editor
		previewTile.setColor(sf::Color(255, 255, 255, 200));
		window.draw(previewTile);
	}

	// draw the keybind overlay if showKeyBindOverlay is true (based on if 'H' key was pressed)
	if (showKeybindOverlay) {
		window.draw(keybindText);
	}
}

void LevelEdit::CreateTile(const sf::Vector2f& position, TileMap& tilemap) {
	int x, y; // declare position variables
	// determine the size of grid cells (gridCellSize), if the collision layer is the active layer, the gridCellSize will be half the normal tileSize otherwise is the standard tileSize
	float gridCellSize = (tilemap.GetActiveLayerIndex() == 3) ? tilemap.tileSize / 2.0f : tilemap.tileSize;
	// convert the world coordinates (position.x/y) into grid coordinates by dividing the gridsize and rounding the result to an int using static_cast<int> this is so the tile will be snapped into the nearest grid cell
	x = static_cast<int>(position.x / gridCellSize);
	y = static_cast<int>(position.y / gridCellSize);
	// if the collision layer is the active layer, set the cell at x, y to 2 (meaning it is collidable), if collision layer isn't place the currently selected tile in tileOptionIndex at x, y
	if (tilemap.GetActiveLayerIndex() == 3) {
		tilemap.SetCollision(x, y, 2);
	}
	else {
		tilemap.SetTile(x, y, tileOptionIndex);
	}
}

void LevelEdit::RemoveTile(const sf::Vector2f& position, TileMap& tilemap) {
	// same as LevelEdit::Create tile except it sets tiles to 1 (meaning it is walkable), or removes them
	int x, y;
	float gridCellSize = (tilemap.GetActiveLayerIndex() == 3) ? tilemap.tileSize / 2.0f : tilemap.tileSize;
	x = static_cast<int>(position.x / gridCellSize);
	y = static_cast<int>(position.y / gridCellSize);

	if (tilemap.GetActiveLayerIndex() == 3) {
		tilemap.SetCollision(x, y, 1);
	}
	else {
		int x = position.x / tilemap.tileSize;
		int y = position.y / tilemap.tileSize;
		tilemap.SetTile(x, y, -1);
	}
}