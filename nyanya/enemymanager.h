#ifndef MOBMANAGER_H
#define MOBMANAGER_H
#include "enemy.h"
#include "tilemap.h"
#include <memory>
#include <vector>
#include <random>

class EnemyManager {
public:
    void Initialize(TileMap& tileMap, int maxEnemies);
    void Update(float deltaTime, const sf::Vector2f& playerPos);
    void Draw(sf::RenderWindow& window);
    void SpawnEnemy(Enemy::Type type);
    void RemoveEnemy(size_t index);
    int GetMaxEnemies() {
        return maxEnemies;
    }
    // getter function so player collision detection and collision detection can access activeEnemies
    const std::vector<std::unique_ptr<Enemy>>& GetActiveEnemies() const {
        return activeEnemies;
    }
    // getter function for the actual count of enemies for the loop that fills enemy count in game.cpp
    int GetActiveEnemiesCount() const {
        return activeEnemies.size();
    }
private:
    // use pointers so enemy objects arent copied (so the same instance is updated and rendered so hitboxes work for every enemy)
    std::vector<std::unique_ptr<Enemy>> activeEnemies;
    TileMap* tileMap; // pointer to the tile map
    int maxEnemies = 20;
    std::default_random_engine rng;
    sf::Vector2f GetRandomTilePosition();
};
#endif