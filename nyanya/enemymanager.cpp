#include "enemymanager.h"

void EnemyManager::Initialize(TileMap& tileMap, int maxEnemies) {
    // store pointer to the TileMap object to use in positioning enemies
    this->tileMap = &tileMap;
    // set the maximum number of enemies allowed at one time
    this->maxEnemies = maxEnemies;
    // seed the random number generator using hardware based random device to ensure randomness when generating enemy spawn positions
    rng.seed(std::random_device{}());
}

sf::Vector2f EnemyManager::GetRandomTilePosition() {
    // create uniform distribution variables for generating random x, y indices within the collision map (GetTileMapLayer(3))
    // std::uniform_int_distribution<int> is used to generate random integers within a specified range, to ensure uniform distribution (each value in the range has an equal probability of being selected)
    std::uniform_int_distribution<int> xDist(0, tileMap->GetTileMapLayer(3)[0].size() - 1);
    std::uniform_int_distribution<int> yDist(0, tileMap->GetTileMapLayer(3).size() - 1);
    int x, y;
    // use rng to generate random x, y indices until a walkable tile is found (tiles are walkable by default)
    do {
        x = xDist(rng);
        y = yDist(rng);
    } 
    // divide x and y by 2 because the collision map 
    while (!tileMap->IsWalkable(x / 2, y / 2));
    // return the tile indices into world coordinates by multiplying by the tileSize and then dividing by 2 to account for collision map's higher resolution/smaller grid cells
    return sf::Vector2f(x * (tileMap->tileSize / 2.0f), y * (tileMap->tileSize / 2.0f));
}

void EnemyManager::SpawnEnemy(Enemy::Type type) {
    // if activeEnemies vector's size is larger than or equal to the number of enemies (maxEnemies) allowed, exit early
    if (activeEnemies.size() >= maxEnemies) return;
    // otherwise, create new Enemy instance using a smart pointer, initialize and load its attributes, then position it at a random walkable tile, and finally push it back into activeEnemies vector
    auto newEnemy = std::make_unique<Enemy>();
    newEnemy->Initialize();
    newEnemy->Load(type);
    newEnemy->sprite.setPosition(GetRandomTilePosition());
    // use std::move to transfer the ownership of the smart pointer to activeEnemies vector
    activeEnemies.push_back(std::move(newEnemy));
}

void EnemyManager::Update(float deltaTime, const sf::Vector2f& playerPos) {
    // iterate through all Enemy instances in activeEnemies vector, call Enemy::Update to give them movement etc.
    for (size_t i = 0; i < activeEnemies.size();) {
        auto& enemy = activeEnemies[i];
        enemy->Update(deltaTime, playerPos);
        // check each Enemy instance's health, if has reached 0 or below, call EnemyManager::Remove function to move that instance to the back of the vector and remove it, otherwise increment i to keep iterating
        if (enemy->health <= 0) {
            RemoveEnemy(i);
        }
        else {
            ++i;
        }
    }
}

void EnemyManager::RemoveEnemy(size_t index) {
    // replaces the enemy at the specified index (which was passed during the EnemyManager::Update funciton) with the last Enemy instance in the vector
    activeEnemies[index] = std::move(activeEnemies.back());
    // erase the enemy that is now at the back of the vector
    activeEnemies.pop_back();
}

void EnemyManager::Draw(sf::RenderWindow& window) {
    // loop through all active enemies in the activeEnemies vector and render them to the window
    // each element in activeEnemies is a std::unique_ptr<Enemy> so use auto& to find this type and access each element by reference, then set it to 'enemy' to be drawn to the screen
    for (auto& enemy : activeEnemies) {
        enemy->Draw(window);
    }
}