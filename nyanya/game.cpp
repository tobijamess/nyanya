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
    // initialize tilemap with its size (50x50 of 64x64 tiles)
    tilemap.Initialize(50, 50);
    // pass this instance of Game instead of creating a Game object and passing that, since that would create a new instance and cause infinite loop
    levelEdit.Initialize(*this);
    levelEdit.Load();
	// initialize and load player data
    player.Initialize();
    player.Load();
	// initialize and load projectile data
    projectile.Initialize();
    projectile.Load(player);
    // initialize and load enemymanager data
    enemymanager.Initialize(GetTileMap(), enemymanager.GetMaxEnemies());
	// initialize and load enemy data
    enemy.Initialize();
    enemy.Load(enemy.GetType());
    // initialize the view size and center
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(player.sprite.getPosition());
    window.setView(view);
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
    // safeguard to skip update funcs and view updating if its the first frame (to prevent view not following player)
    static bool firstFrame = true;
    if (firstFrame) {
        firstFrame = false;
        return;
    }
    // call main update functions
    player.Update(player, enemy, deltaTime);
    enemymanager.Update(deltaTime, player.sprite.getPosition());
    enemy.Update(deltaTime, player.sprite.getPosition());
    projectile.Update(window, player, enemy, deltaTime);
    // get player pos
    sf::Vector2f playerPosition = player.sprite.getPosition();
    // get mouse position in window coords and then convert to world coords
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosWindow);
    // calculate the view center with a slight offset towards the mouse (so mouse can move the view a little bit futher out)
    sf::Vector2f offset = (mousePosWorld - playerPosition) * 0.2f; // 0.2f is sensitivity
    sf::Vector2f newCenter = playerPosition + offset;
    // map bounds
    float mapWidth = 3200.0f;
    float mapHeight = 3200.0f;
    sf::Vector2f viewSize = view.getSize();
    // clamping view center to make sure it stays within map boundaries
    /*newCenter.x = std::max(viewSize.x / 2.f, std::min(newCenter.x, mapWidth - viewSize.x / 2.f));
    newCenter.y = std::max(viewSize.y / 2.f, std::min(newCenter.y, mapHeight - viewSize.y / 2.f));*/
    // update view
    view.setCenter(newCenter);
    window.setView(view);
}

void Game::UpdateView(float deltaTime) {
    
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
        // switch cases for switching between gamemodes based on GameModeSelect() function result
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
        tilemap.Draw(window, *this, levelEdit);
        player.Draw(window);
        enemymanager.Draw(window);
        enemy.Draw(window);
        projectile.Draw(window);
    }
    else if (gameMode == LevelEditor) {
        levelEdit.Draw(window, *this);
        tilemap.Draw(window, *this, levelEdit);
    }
	// copy frame from back buffer to window/screen
    window.display();
}