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
private:
    sf::RenderWindow window;
    sf::Clock clock;

    Player player;
    Enemy enemy;
    Projectile projectile;
    LevelEdit levelEdit;
    
    void Initialize();
    void ProcessEvents();
    void UpdatePlay(float deltaTime);
    void UpdateLevelEditor(Game& game);
    void GameModeSelect();
    void Render();

    // enum object to initialize gamemodes
    GameMode gameMode;
};
#endif