#ifndef LEVELEDIT_H
#define LEVELEDIT_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include "tilemap.h"

class Game;
class TileMap;

class LevelEdit {
public:
	void Initialize(Game& game);
	void Load();
	void Update(sf::RenderWindow& window, Game& game, TileMap& tilemap);
	void Draw(sf::RenderWindow& window, Game& game, TileMap& tilemap);
	void CreateTile(const sf::Vector2f& position, TileMap& tilemap);
	void RemoveTile(const sf::Vector2f& position, TileMap& tilemap);
	// tileOptions getter used in TileMap::DrawAllLayers/Draw functions 
	std::vector<sf::Sprite>& GetTileOptions() { return tileOptions; }
private:
	int tileOptionIndex;
	sf::Texture texture;
	// vector of tiles themselves
	std::vector<sf::Sprite> tiles;
	// vector of different tile images to choose from
	std::vector<sf::Sprite> tileOptions;
	// level editor view
	sf::View view;
	// mouse position when mmb is pressed
	sf::Vector2f initialMousePos;
	// mouse position in world coords
	sf::Vector2f worldMousePos;
	// bool for if user is dragging mouse while holding mmb
	bool isDragging = false;
	// variables/objects for the keybind overlay while in level editor 
	bool showKeybindOverlay = false;
	sf::Font font;
	sf::Text keybindText;
	// variable to show the merged layer preview while in level editor
	bool showMergedLayers = false;
};
#endif