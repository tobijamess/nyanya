#ifndef MOBMANAGER_H
#define MOBMANAGER_H
#include "enemy.h"
#include "tilemap.h"
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
private:
    std::vector<Enemy> activeEnemies;
    TileMap* tileMap; // pointer to the tile map
    int maxEnemies = 100;
    std::default_random_engine rng;
    sf::Vector2f GetRandomTilePosition();
};
#endif