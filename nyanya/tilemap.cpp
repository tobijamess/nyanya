#include "tilemap.h"
#include "game.h"
#include "leveledit.h"

void TileMap::Initialize(int width, int height) {
    // create 3 tile layers with dimensions height x width and each cell in the layer is initialized to -1 which is an empty tile
    tileMap = std::vector<std::vector<int>>(height, std::vector<int>(width, -1));
    tileMap2 = std::vector<std::vector<int>>(height, std::vector<int>(width, -1));
    tileMap3 = std::vector<std::vector<int>>(height, std::vector<int>(width, -1));
    // create the collisionmap with x2 sizes because the collision map needs to have smaller cells for accurate collision with the overall map, each cell is initialized to 1 which is walkable (no collision)
    collisionMap = std::vector<std::vector<int>>(height * 2, std::vector<int>(width * 2, 1));
}

void TileMap::SwitchLayer(int layerIndex) {
    // changes currentLayer to one of the 4 layers depending on the layerIndex, then updates activeLayerIndex to track the current layer, is used in Game::UpdateLevelEditor to switch active layers based on key presses
    switch (layerIndex) {
    case 0: currentLayer = &tileMap; break;
    case 1: currentLayer = &tileMap2; break;
    case 2: currentLayer = &tileMap3; break;
    case 3: currentLayer = &collisionMap; break;
    default: return;
    }
    activeLayerIndex = layerIndex;
    std::cout << "switched to layer " << layerIndex << "\n";
}

void TileMap::SetTile(int x, int y, int tileID) {
    // if x and y are within bounds, set the tile at x, y position in currentLayer to the specified tileID which is changed based on tileOptions vector of sprites which has its' index changed based on scroll wheel input
    // x >= 0 && x < tileMap[0].size() checks the horizontal bounds, y >= 0 && y < tileMap.size() checks the vertical bounds
    // used in LevelEdit::CreateTile
    if (x >= 0 && x < static_cast<int>(tileMap[0].size()) && y >= 0 && y < static_cast<int>(tileMap.size())) {
        (*currentLayer)[y][x] = tileID;
    }
}

int TileMap::GetTile(int x, int y) const {
    // same as TileMap::SetTile but for retrieving the tileID at x, y position in current layer
    // unused right now
    if (x >= 0 && x < static_cast<int>(tileMap[0].size()) && y >= 0 && y < static_cast<int>(tileMap.size())) {
        return (*currentLayer)[y][x];
    }
    return -1; // indicate no tile
}

void TileMap::SetCollision(int x, int y, int value) {
    // if x, y position are within the bounds of the collisionMap, set the tile at x, y position to value (which, when used will be 2 for collision, 1 for walkable)
    // used in LevelEdit::CreateTile/RemoveTile specifically for the collisionMap layer
    if (x < collisionMap[0].size() && y < collisionMap.size()) {
        collisionMap[y][x] = value;
    }
}

bool TileMap::IsWalkable(int x, int y) const {
    // same as TileMap::SetCollision but for returns the collision maps tile at x, y position
    // used in EnemyManager::GetRandomTilePosition which searches for random tiles that are walkable and is then used in EnemyManager::SpawnEnemy to spawn enemies on those randomly chosen tiles
    if (x < collisionMap[0].size() && y < collisionMap.size()) {
        return collisionMap[y][x];
    }
    return false;
}

void TileMap::DrawAllLayers(sf::RenderWindow& window, Game& game, LevelEdit& leveledit) {
    // lambda function drawLayer to draw a single tile layer with parameters layer: 2D array of ints where each int value is a tileID
    auto drawLayer = [&](const std::vector<std::vector<int>>& layer) {
        // iterate over each row and column of tiles in the current layer to process every tile
        for (int y = 0; y < layer.size(); ++y) {
            for (int x = 0; x < layer[y].size(); ++x) {
                // check if the tileID at x, y position is valid, so it must be non-negative and within bounds of available tile options (to prevent invalid tile ID's from rendering unintended tiles)
                int tileID = layer[y][x];
                if (tileID >= 0 && tileID < leveledit.GetTileOptions().size()) {
                    // get the sprite corresponding to tileID from the tileOptions getter function in the LevelEdit class to give each tileID a visual image/sprite
                    sf::Sprite& tile = leveledit.GetTileOptions()[tileID];
                    // set screen position of sprite based on the tile coordinates and the tileSize (which is the size of each tile in px's), to make sure the sprite is positioned properly on the grid
                    tile.setPosition(x * tileSize, y * tileSize);
                    window.draw(tile);
                }
            }
        }
        };
    // call the lambda function to draw all the tile layers in order
    drawLayer(tileMap);
    drawLayer(tileMap2);
    drawLayer(tileMap3);
}

void TileMap::Draw(sf::RenderWindow& window, Game& game, LevelEdit& leveledit, bool showMergedLayers, int activeLayerIndex) {
    // lambda function drawLayer to draw a single tile layer with parameters layer: 2D array of ints where each int value is a tileID, layerIndex: index of the current layer being drawn
    auto drawLayer = [&](const std::vector<std::vector<int>>& layer, int layerIndex) {
        for (int y = 0; y < layer.size(); ++y) {
            for (int x = 0; x < layer[y].size(); ++x) {
                int tileID = layer[y][x];
                if (tileID >= 0 && tileID < leveledit.GetTileOptions().size()) {
                    sf::Sprite& tile = leveledit.GetTileOptions()[tileID];
                    tile.setPosition(x * tileSize, y * tileSize);
                    // set transparency of the tile sprite based on conditions
                    // if showMergedLayers is true and the current layer being drawn (layerIndex) is not the active layer, then the alpha value for this tile is set to 150 (semi-transparent) otherwise, it's fully opaque
                    // examples: !showMergedLayers = 255, showMergedLayers && activeLayerIndex == layerIndex = 255, showMergedLayers && activeLayerIndex != layerIndex = 150
                    tile.setColor(sf::Color(255, 255, 255, (showMergedLayers && activeLayerIndex != layerIndex) ? 150 : 255));
                    window.draw(tile);
                }
            }
        }
    };

    // loop through the first three tile map layers and draw them using the lambda
    for (int layerIndex = 0; layerIndex < 3; ++layerIndex) {
        drawLayer(GetTileMapLayer(layerIndex), layerIndex);
    }

    // loop to draw the collision map tiles if it's the active layer or showMergedLayers is true
    if (activeLayerIndex == 3 || showMergedLayers) {
        for (int y = 0; y < collisionMap.size(); ++y) {
            for (int x = 0; x < collisionMap[y].size(); ++x) {
                // if the tile at collisionMap x, y position has a value of 2 (collidable), draw a semi-transparent red rectangle (collision tile)
                if (collisionMap[y][x] == 2) {
                    sf::RectangleShape collisionTile(sf::Vector2f(tileSize / 2.0f, tileSize / 2.0f));
                    collisionTile.setPosition(x * (tileSize / 2.0f), y * (tileSize / 2.0f));
                    // change collision tile transparency relative to the condition that the active layer is the collision layer
                    collisionTile.setFillColor(sf::Color(255, 0, 0, (activeLayerIndex == 3) ? 150 : 50));
                    window.draw(collisionTile);
                }
            }
        }
    }

    if (game.GetGameMode() == Game::LevelEditor) {
        // assign different values based on the condition that the active layer is collision map or not (because the collision map is the only layer with tiles half the size as it needs more accurate placement)
        float gridTileSize = (activeLayerIndex == 3) ? tileSize / 2.0f : tileSize;
        int gridWidth = (activeLayerIndex == 3) ? collisionMap[0].size() : tileMap[0].size();
        int gridHeight = (activeLayerIndex == 3) ? collisionMap.size() : tileMap.size();

        // loop through the grid and create cells with their specified properties
        for (int y = 0; y < gridHeight; ++y) {
            for (int x = 0; x < gridWidth; ++x) {
                // set the rectangle shape 'gridCell' to match the gridTileSize which will either be 64x64px or 32x32px depending on the above conditions
                sf::RectangleShape gridCell(sf::Vector2f(gridTileSize, gridTileSize));
                gridCell.setPosition(x * gridTileSize, y * gridTileSize);
                gridCell.setFillColor(sf::Color::Transparent);
                gridCell.setOutlineColor(sf::Color(100, 100, 100)); // gray color for grid lines
                gridCell.setOutlineThickness(1.0f);
                window.draw(gridCell);
            }
        }
    }
}