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
    // enum to hold both game modes
    enum GameMode {
        Play,
        LevelEditor
    };
    // getter function to return current gameMode for use in LevelEdit::Update function
    GameMode GetGameMode() const { return gameMode; }
    // enum to hold different scroll wheel states
    enum ScrollWheel {
        ScrollUp,
        ScrollDown,
        None
    };
    // enum object to update scroll wheel states, used in LevelEdit::Update and Game::ProcessEvents functions
    ScrollWheel scrollWheelInput;
    sf::RenderWindow window;
    // getter function to return the current TileMap instance
    TileMap& GetTileMap() { return tilemap; }
private:
    // class instances
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
    // variable to track the time since the last enemy was spawned
    float timeSinceLastSpawn = 0.0f;
    // enum object to initialize and update gamemodes
    GameMode gameMode;
    // enum object to initialize and update gamestates
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
        playButton.setFillColor(sf::Color::White);
        /*settingsButton.setFont(font);
        settingsButton.setString("Settings");
        settingsButton.setPosition(100, 200);*/
        exitButton.setFont(font);
        exitButton.setString("Exit");
        exitButton.setCharacterSize(50);
        exitButton.setPosition(100, 300);
        exitButton.setFillColor(sf::Color::White);
    }

    void HandleInput(sf::RenderWindow& window, Game::GameState& currentstate) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            // get mouse position in window coordinates and convert to mouse position in world coordinates
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);
            // check if the mouse position in world coordinates is within the play buttons boundaries when the left mouse button was clicked, if it was, update the current game state to 'Playing'
            if (playButton.getGlobalBounds().contains(mouseWorldPos)) {
                currentstate = Game::Playing;
            }
            /*else if (settingsButton.getGlobalBounds().contains(mouseWorldPos)) {
                currentstate = Game::Settings;
            }*/
            // same check as the play button but for the exit buttons boundaries, if it was, close the window
            else if (exitButton.getGlobalBounds().contains(mouseWorldPos)) {
                window.close();
            }
        }
    }

    void Draw(sf::RenderWindow& window) {
        // draw the main menus elements in order so buttons are above the background
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
    sf::RectangleShape background;
public:
    void Initialize(sf::RenderWindow& window) {
        background.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
        background.setFillColor(sf::Color(50, 50, 50));
        font.loadFromFile("assets/fonts/font.ttf");
        resumeButton.setFont(font);
        resumeButton.setString("Resume");
        resumeButton.setCharacterSize(50);
        resumeButton.setPosition(100, 100);
        resumeButton.setFillColor(sf::Color::White);
        mainMenuButton.setFont(font);
        mainMenuButton.setString("Main Menu");
        mainMenuButton.setCharacterSize(50);
        mainMenuButton.setPosition(100, 200);
        mainMenuButton.setFillColor(sf::Color::White);
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
        window.draw(background);
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