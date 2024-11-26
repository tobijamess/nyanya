#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "leveledit.h"

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

    enum ScrollWheel {
        ScrollUp,
        ScrollDown,
        None
    };
    ScrollWheel scrollWheelInput;
    // getter function for use in leveledit.cpp's Update() function
    GameMode getGameMode() const { 
        return gameMode;
    }
private:
    sf::RenderWindow window;
    sf::Clock clock;

    Player player;
    Enemy enemy;
    Projectile projectile;
    LevelEdit levelEdit;

    // enum object to initialize gamemodes
    GameMode gameMode;
    
    void Initialize();
    void ProcessEvents();
    void UpdatePlay(float deltaTime);
    void UpdateLevelEditor();
    void GameModeSelect();
    void Render();
};
#endif