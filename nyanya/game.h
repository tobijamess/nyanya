#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "leveledit.h"
#include "tilemap.h"
#include "enemymanager.h"

class Game {
public:
    Game();
    ~Game();
    void Run();
    // enum which holds both game states
    enum GameMode {
        Play,
        LevelEditor
    };
    // getter function for use in leveledit.cpp's Update() function
    GameMode getGameMode() const {
        return gameMode;
    }
    enum ScrollWheel {
        ScrollUp,
        ScrollDown,
        None
    };
    ScrollWheel scrollWheelInput;  
    sf::RenderWindow window;
    TileMap& GetTileMap() {
        return tilemap;
    }
private:
    sf::Clock clock;
    sf::View view;

    Player player;
    Enemy enemy;
    Projectile projectile;
    LevelEdit levelEdit;
    TileMap tilemap;
    EnemyManager enemymanager;

    // enum object to initialize gamemodes
    GameMode gameMode;
    
    void Initialize();
    void ProcessEvents();
    void UpdatePlay(float deltaTime);
    void UpdateView(float deltaTime);
    void UpdateLevelEditor();
    void GameModeSelect();
    void Render();
};
#endif