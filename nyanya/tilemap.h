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
	float tileSize = 64.0f;
	// tilemap initialization and helper functions
	void Initialize(int width, int height);
	void SwitchLayer(int layerIndex);
	void SetTile(int x, int y, int tileID);
	int GetTile(int x, int y) const;
	void SetCollision(int x, int y, bool walkable);
	bool IsWalkable(int x, int y) const;
	void DrawAllLayers(sf::RenderWindow& window, Game& game, LevelEdit& leveledit);
	void Draw(sf::RenderWindow& window, Game& game, LevelEdit& leveledit);
	// getter function to return tileMap
	std::vector<std::vector<int>>& GetTileMapLayer() {
		return *currentLayer;
	}
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
	// vector of vectors to store collision map
	std::vector<std::vector<bool>> collisionMap;
	// pointer to current active tilemap layer
	std::vector<std::vector<int>>* currentLayer = &tileMap;
};
#endif