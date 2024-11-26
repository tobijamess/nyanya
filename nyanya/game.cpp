#include "game.h"
// game constructor
Game::Game()
    : window(sf::VideoMode(1280, 720), "Game Window")
    , gameMode(Play)
    , scrollWheelInput(None) {
    // initialize game function which holds all sub-initialize functions (player.initialize() etc.)
    Initialize();
}

Game::~Game() {
    // clean up if needed
}

void Game::Initialize() {
    // pass this instance of Game instead of creating a Game object and passing that, since that would create a new instance and cause infinite loop
    levelEdit.Initialize(*this);
    levelEdit.Load();
	// initialize and load player data
    player.Initialize();
    player.Load();
	// initialize and load projectile data
    projectile.Initialize();
    projectile.Load(player);
	// initialize and load enemy data
    enemy.Initialize();
    enemy.Load();
}

void Game::ProcessEvents() {
    sf::Event event;
    // set scroll wheel input to none by default
    scrollWheelInput = None;
	// checking window events
    while (window.pollEvent(event)) {
        // if application window exit is pressed, close window
        switch (event.type) {
        case  sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                if (event.mouseWheelScroll.delta > 0) {
                    scrollWheelInput = ScrollUp;
                }
                else {
                    scrollWheelInput = ScrollDown;
                }
            }
        }
    }
}

// update functions for gamestate 'play'
void Game::UpdatePlay(float deltaTime) {
    player.Update(player, enemy, deltaTime);
    enemy.Update(deltaTime);
    projectile.Update(window, player, enemy, deltaTime);
}

// update functions for gamestate 'level editor'
void Game::UpdateLevelEditor() {
    levelEdit.Update(window, *this);
}

void Game::GameModeSelect() {
    // was T pressed in the last update frame
    static bool wasTPressed = false;
    // when T is pressed, toggle between gamemode play and leveleditor
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
       // if you pressed T and it wasn't pressed last update frame, toggle to next gamemode
       if (!wasTPressed){
            if (gameMode == Play) {
                gameMode = LevelEditor;
                std::cout << "gamemode leveleditor\n";
            }
            else {
                gameMode = Play;
                std::cout << "gamemode play\n";
            }
       }
       wasTPressed = true;
    }
    else {
        wasTPressed = false;
    }
}

void Game::Run() { 
    while (window.isOpen()) {
        ProcessEvents();
        // restart the timer at the beginning of every window frame
        // store the timer value in deltaTime variable for use in other functions (player.Update() etc.)
        float deltaTime = clock.restart().asSeconds();
        GameModeSelect();
        switch (gameMode) {
        case Play:
            UpdatePlay(deltaTime);
            break;
        case LevelEditor:
            UpdateLevelEditor();
            break;
        }
        Render();
    }
}

void Game::Render() {
	// default window background color
    window.clear(sf::Color::Black);
    // determine what is drawn to the application window based on game states
    if (gameMode == Play) {
        // calling draw functions to literally draw the sprite or shape to the render window
        levelEdit.Draw(window, *this);
        player.Draw(window);
        enemy.Draw(window);
        projectile.Draw(window);
    }
    else if (gameMode == LevelEditor) {
        levelEdit.Draw(window, *this);
    }
	// copy frame from back buffer to window/screen
    window.display();
}