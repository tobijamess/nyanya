#include "tilemap.h"
#include "game.h"
#include "leveledit.h"

void TileMap::Initialize(int width, int height) {
    // initialize the tilemaps with default sizes and default values, set default tileID to 0 makes sure the tilemap is completely empty
    tileMap = std::vector<std::vector<int>>(height, std::vector<int>(width, -1));
    tileMap2 = std::vector<std::vector<int>>(height, std::vector<int>(width, -1));
    tileMap3 = std::vector<std::vector<int>>(height, std::vector<int>(width, -1));
    // initialize the collisionmap with x2 sizes (to choose quadrants of tiles that will be collidable), 1 makes sure they are walkable by default (no collision)
    collisionMap = std::vector<std::vector<int>>(height * 2, std::vector<int>(width * 2, 1));
}

void TileMap::SwitchLayer(int layerIndex) {
    // switch cases to change between layers 
    switch (layerIndex) {
    case 0:
        currentLayer = &tileMap;
        break;
    case 1:
        currentLayer = &tileMap2;
        break;
    case 2:
        currentLayer = &tileMap3;
        break;
    case 3:
        currentLayer = &collisionMap;
        break;
    default:
        return;
    }
    activeLayerIndex = layerIndex;
    std::cout << "switched to layer " << layerIndex << "\n";
}

void TileMap::SetTile(int x, int y, int tileID) {
    // if x and y exist and are within the valid range of the tileMap dimensions
    // x >= 0 && x < tileMap[0].size() checks the horizontal range, y >= 0 && y < tileMap.size() checks the vertical range
    if (x >= 0 && x < static_cast<int>(tileMap[0].size()) && y >= 0 && y < static_cast<int>(tileMap.size())) {
        // the position is valid, update the tile at x, y with the specified tileID
        (*currentLayer)[y][x] = tileID;
    }
    // example usage: tileMap.SetTile(3, 4, 5);
}

int TileMap::GetTile(int x, int y) const {
    // same as SetTile() but returns the tileID stored in tileMap[y][x]
    if (x >= 0 && x < static_cast<int>(tileMap[0].size()) && y >= 0 && y < static_cast<int>(tileMap.size())) {
        return (*currentLayer)[y][x];
    }
    return -1; // indicate no tile
    // example usage: int tileID = tileMap.GetTile(3, 4);
}

void TileMap::SetCollision(int x, int y, int value) {
    // convert x and y to collision grid indices (x2)
    x *= 2;
    y *= 2;
    for (int offsetY = 0; offsetY < 2; ++offsetY) {
        for (int offsetX = 0; offsetX < 2; ++offsetX) {
            // if x and y exist and are within the bounds of the collisionMap
            if (x + offsetX < collisionMap[0].size() && y + offsetY < collisionMap.size()) {
                // valid so update the collisionMap with bool value
                collisionMap[y + offsetY][x + offsetX] = value;
            }
            // example usage: tileMap.SetCollision(3, 4, true);
        }
    }
}

bool TileMap::IsWalkable(int x, int y) const {
    x *= 2;
    y *= 2;
    // same as SetCollision() but returns value of collisionMap[y][x] which will either be true/walkable or false/non-walkable
    for (int offsetY = 0; offsetY < 2; ++offsetY) {
        for (int offsetX = 0; offsetX < 2; ++offsetX) {
            if (x + offsetX < collisionMap[0].size() && y + offsetY < collisionMap.size()) {
                return collisionMap[y + offsetY][x + offsetX] == 1;
            }
        }
    }
    return false;
    // example usage: bool walkable = tileMap.IsWalkable(3, 4);
}

void TileMap::DrawAllLayers(sf::RenderWindow& window, Game& game, LevelEdit& leveledit) {
    auto drawLayer = [&](const std::vector<std::vector<int>>& layer) {
        for (int y = 0; y < layer.size(); ++y) {
            for (int x = 0; x < layer[y].size(); ++x) {
                int tileID = layer[y][x];
                if (tileID >= 0 && tileID < leveledit.GetTileOptions().size()) {
                    sf::Sprite& tile = leveledit.GetTileOptions()[tileID];
                    tile.setPosition(x * tileSize, y * tileSize);
                    window.draw(tile);
                }
            }
        }
        };
    // draw each layer in order
    drawLayer(tileMap);
    drawLayer(tileMap2);
    drawLayer(tileMap3);
}

void TileMap::Draw(sf::RenderWindow& window, Game& game, LevelEdit& leveledit) {
    if (currentLayer) {
        auto drawLayer = [&](const std::vector<std::vector<int>>& layer, float gridTileSize) {
            // iterate over rows and then columns
            for (int y = 0; y < layer.size(); ++y) {
                for (int x = 0; x < layer[y].size(); ++x) {
                    // fetch tileID from tileMap (tileID is int which represents which tile image to draw)
                    int tileID = layer[y][x];
                    // validation
                    if (tileID >= 0 && tileID < leveledit.GetTileOptions().size()) {
                        // set tile sprite to the tile image associated with that tileID
                        sf::Sprite& tile = leveledit.GetTileOptions()[tileID];
                        // set sprite position in world coords based on tiles x any y index (x * tileSize, y * tileSize converts tile indices to pixel coords)
                        tile.setPosition(x * tileSize, y * tileSize);
                        tile.setScale(tileSize / 64.0f, tileSize / 64.0f);
                        window.draw(tile);
                    }
                }
            }
            };
        // if active layer is not collision map layer
        if (activeLayerIndex != 3) {
            // use drawLayer helper func to draw current layer
            drawLayer(*currentLayer, tileSize);
        }
        else {
            // iterate over rows and then columns of collision tilemap
            for (int y = 0; y < collisionMap.size(); ++y) {
                for (int x = 0; x < collisionMap[y].size(); ++x) {
                    // check collisionMap value to determine if tile is non-walkable (value 2)
                    if (collisionMap[y][x] == 2) {
                        // tileSize / 2.0f because collision map tiles are half the size of normal tiles
                        sf::RectangleShape highlight(sf::Vector2f(tileSize / 2.0f, tileSize / 2.0f));
                        highlight.setPosition(x * (tileSize / 2.0f), y * (tileSize / 2.0f));
                        highlight.setFillColor(sf::Color(255, 0, 0, 50)); // semi-transparent red fill for collidable tiles
                        window.draw(highlight);
                    }
                }
            }
        }
    }
    // gridline drawing 
    float gridTileSize = (activeLayerIndex == 3) ? tileSize / 2.0f : tileSize;
    int gridWidth = (activeLayerIndex == 3) ? collisionMap[0].size() : tileMap[0].size();
    int gridHeight = (activeLayerIndex == 3) ? collisionMap.size() : tileMap.size();

    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            // set the rectangle shape 'gridCell' to match the tileSize from TileMap class (64x64)
            sf::RectangleShape gridCell(sf::Vector2f(gridTileSize, gridTileSize));
            gridCell.setPosition(x * gridTileSize, y * gridTileSize);
            gridCell.setFillColor(sf::Color::Transparent);
            gridCell.setOutlineColor(sf::Color(100, 100, 100)); // gray color for grid lines
            gridCell.setOutlineThickness(1.0f);
            // draw gridlines around the tile
            window.draw(gridCell);
        }
    }
}
