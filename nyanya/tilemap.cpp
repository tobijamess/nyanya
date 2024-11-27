#include "tilemap.h"
#include "game.h"
#include "leveledit.h"

void TileMap::Initialize(int width, int height) {
	// initialize the tilemap with default sizes and default values, -1 makes sure the tilemap is completely empty
	tileMap = std::vector<std::vector<int>>(height, std::vector<int>(width, -1));
	// initialize the collisionmap with default sizes and values, true makes sure they are walkable by default (no collision)
	collisionMap = std::vector<std::vector<bool>>(height, std::vector<bool>(width, true));
}

void TileMap::SetTile(int x, int y, int tileID) {
    // if x and y exist and are within the valid range of the tileMap dimensions
    // x >= 0 && x < tileMap[0].size() checks the horizontal range, y >= 0 && y < tileMap.size() checks the vertical range
    if (x >= 0 && x < static_cast<int>(tileMap[0].size()) && y >= 0 && y < static_cast<int>(tileMap.size())) {
        // the position is valid, update the tile at x, y with the specified tileID
        tileMap[y][x] = tileID;
    }
    // example usage: tileMap.SetTile(3, 4, 5);
}

int TileMap::GetTile(int x, int y) const {
    // same as SetTile() but returns the tileID stored in tileMap[y][x]
    if (x >= 0 && x < static_cast<int>(tileMap[0].size()) && y >= 0 && y < static_cast<int>(tileMap.size())) {
        return tileMap[y][x];
    }
    return -1; // indicate no tile
    // example usage: int tileID = tileMap.GetTile(3, 4);
}

void TileMap::SetCollision(int x, int y, bool walkable) {
    // if x and y exist and are within the bounds of the collisionMap
    if (x >= 0 && x < collisionMap[0].size() && y >= 0 && y < collisionMap.size()) {
        // valid so update the collisionMap with bool value
        collisionMap[y][x] = walkable;
    }
    // example usage: tileMap.SetCollision(3, 4, true);
}

bool TileMap::IsWalkable(int x, int y) const {
    // same as SetCollision() but returns value of collisionMap[y][x] which will either be true/walkable or false/non-walkable
    if (x >= 0 && x < collisionMap[0].size() && y >= 0 && y < collisionMap.size()) {
        return collisionMap[y][x];
    }
    return false; // out of bounds tiles are not walkable
    // example usage: bool walkable = tileMap.IsWalkable(3, 4);
}

void TileMap::Draw(sf::RenderWindow& window, Game& game, LevelEdit& leveledit) {
    // iterate over rows and then columns
    for (int y = 0; y < tileMap.size(); ++y) {
        for (int x = 0; x < tileMap[y].size(); ++x) {
            // fetch tileID from tileMap (tileID is int which represents which tile image to draw)
            int tileID = tileMap[y][x];
            // validation
            if (tileID >= 0 && tileID < leveledit.GetTileOptions().size()) {
                // set tile sprite to the tile image associated with that tileID
                sf::Sprite& tile = leveledit.GetTileOptions()[tileID];
                // set sprite position in world coords based on tiles x any y index (x * tileSize, y * tileSize converts tile indices to pixel coords)
                tile.setPosition(x * tileSize, y * tileSize);
                window.draw(tile);
            }
            // set the rectangle shape 'gridCell' to match the tileSize from TileMap class (64x64)
            sf::RectangleShape gridCell(sf::Vector2f(tileSize, tileSize));
            gridCell.setPosition(x * tileSize, y * tileSize);
            gridCell.setFillColor(sf::Color::Transparent);
            gridCell.setOutlineColor(sf::Color(100, 100, 100)); // grey color for gridlines
            gridCell.setOutlineThickness(1.0f);
            // draw gridlines around the tile
            window.draw(gridCell);
            // check collisionMap value to determine if tile is non-walkable (has collision)
            if (!collisionMap[y][x]) {
                sf::RectangleShape highlight(sf::Vector2f(tileSize, tileSize));
                highlight.setPosition(x * tileSize, y * tileSize);
                highlight.setFillColor(sf::Color(255, 0, 0, 100)); // semi-transparent red fill for collidable tiles
                window.draw(highlight);
            }
        }
    }
}