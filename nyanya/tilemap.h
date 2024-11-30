#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

class LevelEdit;
class Game;

class TileMap {
public:
	// default tile size in px
	float tileSize = 32.0f;
	void Initialize(int width, int height);
	void SwitchLayer(int layerIndex);
	void SetTile(int x, int y, int tileID);
	int GetTile(int x, int y) const;
	void SetCollision(int x, int y, int value);
	bool IsWalkable(int x, int y) const;
	void DrawAllLayers(sf::RenderWindow& window, Game& game, LevelEdit& leveledit);
	void Draw(sf::RenderWindow& window, Game& game, LevelEdit& leveledit, bool showMergedLayers, int activeLayerIndex);
	// getter function to return the active tilemap layer (1, 2, 3 or collision) 
	// unused right now
	std::vector<std::vector<int>>& GetActiveMapLayer() {
		return *currentLayer;
	}
	// getter function to return the tile map layers based on their index number
	// used in drawLayer lambda's and for always getting the collision map layer in EnemyManager::GetRandomTilePosition
	const std::vector<std::vector<int>>& GetTileMapLayer(int layerIndex) const {
		switch (layerIndex) {
		case 0: return tileMap;
		case 1: return tileMap2;
		case 2: return tileMap3;
		case 3: return collisionMap;
		default: throw std::out_of_range("Invalid layer index");
		}
	}
	// getter function to return the index number of the current active layer
	// used in LevelEdit functions for placing/removing tiles as well as updating the tileOptions preview to follow the cursor on the active layer
	int GetActiveLayerIndex() const {
		return activeLayerIndex;
	}
private:
	// tracks the active layer index (0-3)
	int activeLayerIndex = 0;
	// vectors of vectors to store tilemaps, each int is a tile ID
	std::vector<std::vector<int>> tileMap;
	std::vector<std::vector<int>> tileMap2;
	std::vector<std::vector<int>> tileMap3;
	std::vector<std::vector<int>> collisionMap;
	// pointer to current active tilemap layer for use in getter functions
	std::vector<std::vector<int>>* currentLayer = &tileMap;
};
#endif