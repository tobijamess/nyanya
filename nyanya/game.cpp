#include "game.h"

// game constructor to setup the render window, set default gamestate and gamemode, and make sure no unintended scroll wheel input is registered
Game::Game()
    : window(sf::VideoMode(1280, 720), "Game Window")
    , gameMode(Play)
    , gameState(MainMenu)
    , scrollWheelInput(None)
{
    // initialize game function which holds all initialize and load functions
    Initialize();
}

Game::~Game() {
    // clean up if needed
}

void Game::Initialize() {
    // initialize tilemap with its size in tiles e.g. (50, 50) would be 50 tiles wide and 50 tiles high
    tilemap.Initialize(100, 100);
    // pass this instance of Game class instead of creating a new Game class object and passing that, since that would create a new instance and cause infinite loop
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
    enemy.Load(enemy.GetType());
    // initialize and load enemymanager data
    enemymanager.Initialize(GetTileMap(), enemymanager.GetMaxEnemies());
    // initialize the view as the same size as the window, and set its' center to the player's position so the player sprite is always at the center
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(player.sprite.getPosition());
    window.setView(view);
}

void Game::ProcessEvents() {
    sf::Event event;
    // loop to continuously check the event queue for any pending scroll wheel inputs or events
    while (window.pollEvent(event)) {
        switch (event.type) {
        // if user pressed the close button on the application window, call window.close function to terminate the window
        case sf::Event::Closed:
            window.close();
            break;
        // if user made scroll wheel inputs, and it was a positive scroll (mouseWheelScroll.delta > 0 determines this) then set scrollWheelInput enum object to ScrollUp, otherwise set it to ScrollDown
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

void Game::UpdatePlay(float deltaTime) {
    // call main update functions
    player.Update(player, enemymanager, deltaTime); 
    enemymanager.Update(deltaTime, player.sprite.getPosition());
    projectile.Update(window, player, enemymanager, deltaTime);
    // timeSinceLastSpawn tracks the time between each enemy spawn
    static float timeSinceLastSpawn = 0.0f;
    // timeSinceLastSpawn has deltaTime added to it instead of an increment because otherwise an enemy would spawn every frame rather than every second
    timeSinceLastSpawn += deltaTime;
    // if timeSinceLastSpawn is greater than or equal to 1.0f (so a second has passed) and the number of active enemies is less than the maximum limit, spawn an enemy of type Basic and then reset timeSinceLastSpawn value
    if (timeSinceLastSpawn >= 1.0f && enemymanager.GetActiveEnemiesCount() < enemymanager.GetMaxEnemies()) {
        enemymanager.SpawnEnemy(Enemy::Type::Basic);
        timeSinceLastSpawn = 0.0f;
    }

    // get the player's current position
    sf::Vector2f playerPosition = player.sprite.getPosition();
    // get mouse position in window coordinates and then convert it to world coordinates
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosWindow);
    // calculate the new view center with a slight offset towards the mouse so the player can use their mouse to dynamically pan the camera further out
    sf::Vector2f offset = (mousePosWorld - playerPosition) * 0.2f; // 0.2f is sensitivity of the panning
    sf::Vector2f newCenter = playerPosition + offset;
    // map bounds, adjust this if you want to increase/decrease the map size
    float mapWidth = 3200.0f;
    float mapHeight = 3200.0f;
    // update view with dynamic panning taken into account
    sf::Vector2f viewSize = view.getSize();
    view.setCenter(newCenter);
    window.setView(view);
}

void Game::UpdateLevelEditor() {
    // keyboard input toggles to call TileMap::SwitchLayer function that updates the currentLayer object based on the layerIndex passed, then call LevelEdit::Update to update the tile layer that was switched to
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) { tilemap.SwitchLayer(0); }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) { tilemap.SwitchLayer(1); }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) { tilemap.SwitchLayer(2); }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) { tilemap.SwitchLayer(3); }
    levelEdit.Update(window, *this, GetTileMap());
}

void Game::GameModeSelect() {
    // when 'T' key is pressed, toggle between gamemode play and leveleditor (check LevelEdit::Update for commenting)
    static bool wasTPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
        if (!wasTPressed) {
            if (gameMode == Play) {
                gameMode = LevelEditor;
            }
            else {
                gameMode = Play;
            }
        }
        wasTPressed = true;
    }
    else {
        wasTPressed = false;
    }
}

void Game::Run() {
    // create a new instance of the cMainMenu and cPauseMenu classes and initialize them
    mainMenu = new cMainMenu();
    mainMenu->Initialize(window);
    pauseMenu = new cPauseMenu();
    pauseMenu->Initialize(window);

    while (window.isOpen()) {
        // call process events for processing scroll wheel inputs and whether the application window is closed or not
        ProcessEvents();
        // reset the clock and calculate the elapsed time since the last frame in seconds, then store it in deltaTime variable for use in functions that require time-based updates e.g. movement
        float deltaTime = clock.restart().asSeconds();
        // switch between game states based on the user inputs in different menus (pressing play etc.)
        switch (gameState) {
        case MainMenu:
            // processes user input specific to the main menu
            mainMenu->HandleInput(window, gameState);
            break;
        case Playing:
            // call function that processes user toggling between different game modes (play and level editor)
            GameModeSelect();
            // if the user presses 'Esc' during game state 'Playing' then set gameState to PauseMenu
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) gameState = PauseMenu;
            // checks for a result of the Game::GameModeSelect function and calls either of the game mode specific update functions based on that result
            if (gameMode == Play) UpdatePlay(deltaTime);
            else if (gameMode == LevelEditor) UpdateLevelEditor();
            break;
        case PauseMenu:
            // processes user input specific to the pause menu
            pauseMenu->HandleInput(window, gameState);
            break;
            // no game over sequence yet
        case GameOver:
            break;
        }
        // call Game::Render function which determines what is drawn to the application window based on game states
        Render();
    }
}

void Game::Render() {
    // clear the screen to prepare for drawing the current frame
    window.clear();
    // determine what is drawn to the application window based on game states
    switch (gameState) {
    case MainMenu:
        // reset the view to the default view which is centered on the window not the player to then call cMainMenu::Draw which displays the main menu
        window.setView(window.getDefaultView());
        mainMenu->Draw(window);
        break;
    case Playing:
        if (gameMode == Play) {
            // if the current gameMode is 'Play', draw the game map layers, the player, all active enemies, and projectiles
            tilemap.Draw(window, *this, levelEdit, false, tilemap.GetActiveLayerIndex());
            player.Draw(window);
            enemymanager.Draw(window);
            projectile.Draw(window);
        }
        else if (gameMode == LevelEditor) {
            // otherwise, if the current gameMode is 'LevelEditor' only call the level editor specific LevelEdit::Draw function
            levelEdit.Draw(window, *this, tilemap);
        }
        break;
    case PauseMenu:
        // reset the view the same as the case for MainMenu, then call cPauseMenu::Draw function
        window.setView(window.getDefaultView());
        pauseMenu->Draw(window);
        break;
    case GameOver:
        // handle GameOver state rendering when implemented
        break;
    }
    // copy frame from back buffer to the application window
    window.display();
}