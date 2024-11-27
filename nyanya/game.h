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

class cMainMenu;
class cPauseMenu;

class Game {
public:
    Game();
    ~Game();
    void Run();
    // enum to hold different game states
    enum GameState {
        MainMenu,
        Playing,
        PauseMenu,
        Settings,
        GameOver
    };
    // getter function for use in switch cases in ProcessEvents()
    GameState GetGameState() const {
        return gameState;
    }
    // enum which holds both game modes
    enum GameMode {
        Play,
        LevelEditor
    };
    // getter function for use in leveledit's Update() function
    GameMode GetGameMode() const {
        return gameMode;
    }
    // enum to hold different scroll wheel directions
    enum ScrollWheel {
        ScrollUp,
        ScrollDown,
        None
    };
    ScrollWheel scrollWheelInput; 
    sf::RenderWindow window;
    // tilemap getter function
    TileMap& GetTileMap() {
        return tilemap;
    }
private:
    // object initialization
    sf::Clock clock;
    sf::View view;
    Player player;
    Enemy enemy;
    Projectile projectile;
    LevelEdit levelEdit;
    TileMap tilemap;
    EnemyManager enemymanager;
    cMainMenu* mainMenu;
    cPauseMenu* pauseMenu;

    // for enemy spawning
    float timeSinceLastSpawn = 0.0f;
    float spawnInterval = 10.0f; // spawn every 1 second

    // enum object to initialize gamemodes
    GameMode gameMode;
    // enum object to initialize gamestates
    GameState gameState;
    
    void Initialize();
    void ProcessEvents();
    void UpdatePlay(float deltaTime);
    void UpdateLevelEditor();
    void GameModeSelect();
    void Render();
};

class cMainMenu {
private:
    sf::Font font;
    sf::Text playButton;
    sf::Text settingsButton;
    sf::Text exitButton;
    sf::RectangleShape background;
public:
    void Initialize(sf::RenderWindow& window) {
        // initialize a background rectangle with a default color CHANGE TO SPRITE OR BACKGROUND IMAGE LATER
        background.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
        background.setFillColor(sf::Color(50, 50, 50));
        // load fonts and font settings
        font.loadFromFile("assets/fonts/font.ttf");
        playButton.setFont(font);
        playButton.setString("Play");
        playButton.setCharacterSize(50);
        playButton.setPosition(540, 300);
        playButton.setFillColor(sf::Color::Red);
        /*settingsButton.setFont(font);
        settingsButton.setString("Settings");
        settingsButton.setPosition(100, 200);*/
        exitButton.setFont(font);
        exitButton.setString("Exit");
        exitButton.setCharacterSize(50);
        exitButton.setPosition(100, 300);
        exitButton.setFillColor(sf::Color::Red);
    }
    void HandleInput(sf::RenderWindow& window, Game::GameState& currentstate) {
        // check if lmb is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            // get mouse pos in window and then get mouse pos in world coords
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);
            // if the boundary of the play button contains the mouse
            if (playButton.getGlobalBounds().contains(mouseWorldPos)) {
                // change game state to playing
                currentstate = Game::Playing;
            }
            /*else if (settingsButton.getGlobalBounds().contains(mouseWorldPos)) {
                currentstate = Game::Settings;
            }*/
            else if (exitButton.getGlobalBounds().contains(mouseWorldPos)) {
                window.close();
            }
        }
        
    }
    void Draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(playButton);
        // window.draw(settingsButton);
        window.draw(exitButton);
    }
};

class cPauseMenu {
private:
    sf::Font font;
    sf::Text resumeButton;
    /*sf::Text settingsButton;*/
    sf::Text mainMenuButton;
public:
    void Initialize() {
        font.loadFromFile("assets/fonts/font.ttf");
        resumeButton.setFont(font);
        resumeButton.setString("Resume");
        resumeButton.setCharacterSize(50);
        resumeButton.setPosition(100, 100);
        resumeButton.setFillColor(sf::Color::Blue);
        mainMenuButton.setFont(font);
        mainMenuButton.setString("Main Menu");
        mainMenuButton.setCharacterSize(50);
        mainMenuButton.setPosition(100, 200);
        resumeButton.setFillColor(sf::Color::Blue);
    }
    void HandleInput(sf::RenderWindow& window, Game::GameState& currentstate) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);
            if (resumeButton.getGlobalBounds().contains(mouseWorldPos)) {
                currentstate = Game::Playing;
            }
            else if (mainMenuButton.getGlobalBounds().contains(mouseWorldPos)) {
                currentstate = Game::MainMenu;
            }
        }
    }
    void Draw(sf::RenderWindow& window) {
        window.draw(resumeButton);
        window.draw(mainMenuButton);
    }
};

//class cSettingsMenu {
//private:
//    sf::Font font;
//    sf::Text returnButton;
//public:
//    void Initialize() {
//        font.loadFromFile("assets/fonts/font.ttf");
//        returnButton.setFont(font);
//        returnButton.setString("Resume");
//        returnButton.setPosition(100, 100);
//    }
//    void HandleInput(sf::RenderWindow& window, Game::GameState& currentstate) {
//        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
//            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);
//            if (returnButton.getGlobalBounds().contains(mouseWorldPos)) {
//                currentstate = Game::MainMenu;
//            }
//        }
//    }
//    void Draw(sf::RenderWindow& window) {
//        window.draw(returnButton);
//    }
//};
#endif