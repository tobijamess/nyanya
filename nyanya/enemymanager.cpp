#include "enemymanager.h"

void EnemyManager::Initialize(TileMap& tileMap, int maxEnemies) {
    // store pointer to TileMap to use later in enemy positioning
    this->tileMap = &tileMap;
    // set max enemies to control number of enemies that can spawn
    this->maxEnemies = maxEnemies;
    rng.seed(std::random_device{}()); // seed rng
}

// random tile selection for random enemy spawn points
sf::Vector2f EnemyManager::GetRandomTilePosition() {
    // collision map is higher resolution, so adjust the range for x and y
    std::uniform_int_distribution<int> xDist(0, tileMap->GetTileMapLayer(3)[0].size() - 1);
    std::uniform_int_distribution<int> yDist(0, tileMap->GetTileMapLayer(3).size() - 1);
    int x, y;
    // loop until valid tile that IsWalkable (doesn't collide) and isnt empty or unusable (GetTile(x, y) == -1)
    do {
        x = xDist(rng);
        y = yDist(rng);
    } while (!tileMap->IsWalkable(x / 2, y / 2));
    // return the position in world space; adjust by tileSize / 2 for collision map resolution
    return sf::Vector2f(x * (tileMap->tileSize / 2.0f), y * (tileMap->tileSize / 2.0f));
}

// function to spawn enemy at random tile position
void EnemyManager::SpawnEnemy(Enemy::Type type) {
    // if activeEnemies vector is more than maxEnemies allowed, exit early
    if (activeEnemies.size() >= maxEnemies) return;
    // otherwise, create new enemy initialize and load it, then set random spawn position, and finally push it back into activeEnemies vector
    auto newEnemy = std::make_unique<Enemy>();
    newEnemy->Initialize();
    newEnemy->Load(type);
    newEnemy->sprite.setPosition(GetRandomTilePosition());
    activeEnemies.push_back(std::move(newEnemy));
}

void EnemyManager::Update(float deltaTime, const sf::Vector2f& playerPos) {
    for (size_t i = 0; i < activeEnemies.size();) {
        auto& enemy = activeEnemies[i];
        enemy->Update(deltaTime, playerPos);
        if (enemy->health <= 0) {
            RemoveEnemy(i);
        }
        else {
            ++i;
        }
    }
}

void EnemyManager::RemoveEnemy(size_t index) {
    // move current enemy in activeEnemies index to the back of the list
    activeEnemies[index] = std::move(activeEnemies.back());
    // erase the enemy that is now at the back of the list
    activeEnemies.pop_back();
}

void EnemyManager::Draw(sf::RenderWindow& window) {
    for (auto& enemy : activeEnemies) {
        enemy->Draw(window);
    }
}