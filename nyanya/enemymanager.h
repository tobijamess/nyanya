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
    // getter function to return the value that represents the maximum number of enemies that can exist, used in Game::Initialize/Update
    int GetMaxEnemies() { return maxEnemies; }
    // getter function to return activeEnenmies so player collision detection and collision detection can access it
    const std::vector<std::unique_ptr<Enemy>>& GetActiveEnemies() const { return activeEnemies; }
    // getter function to return the amount of enemies for a loop that fills the active enemy count in Game::UpdatePlay
    int GetActiveEnemiesCount() const { return activeEnemies.size(); }
private:
    // use pointers so Enemy objects arent copied (so the same Enemy instance is updated and rendered each time) (makes sure hitboxes work for every enemy)
    std::vector<std::unique_ptr<Enemy>> activeEnemies;
    // pointer to TileMap class to get tile sizes and tilemap functions for EnemyManager::GetRandomTilePosition function
    TileMap* tileMap;
    // value that corresponds to the maximum amount of enemies that can exist
    int maxEnemies = 20;
    // create instance of the standard random number generator class for use in EnemyManager::GetRandomTilePosition function
    std::default_random_engine rng;
    sf::Vector2f GetRandomTilePosition();
};
#endif