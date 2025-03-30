#include "Game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode(windowWidth, windowHeight), windowTitle, sf::Style::Close), currentState(GameState::MainMenu), currentPlayer(1), isGameOver(false), winner(0)
{
    // Set window properties
    window.setFramerateLimit(60);

    // Initialize resource managers
    resourceManager = std::make_unique<ResourceManager>();
    resourceManager->loadDefaultResources();

    soundManager = std::make_unique<SoundManager>();
    soundManager->loadDefaultSounds();

    // Initialize theme manager
    themeManager = std::make_unique<ThemeManager>();
    themeManager->initializeDefaultThemes();

    // Initialize game board
    gameBoard = std::make_unique<Board>(*resourceManager, *soundManager);
    gameBoard->setPosition((windowWidth - gameBoard->getBounds().width) / 2.0f,
                           (windowHeight - gameBoard->getBounds().height) / 2.0f);
    gameBoard->setTheme(themeManager->getCurrentTheme().xTextureName);

    // Initialize main menu
    mainMenu = std::make_unique<Menu>(*resourceManager, *soundManager);
    mainMenu->addTitle("TicTacTech");
    mainMenu->addButton("Play Game", [this]()
                        { 
        currentState = GameState::Playing;
        mainMenu->setVisible(false);
        resetGame(); });
    mainMenu->addButton("Settings", [this]()
                        { 
        currentState = GameState::Settings;
        mainMenu->setVisible(false);
        settingsMenu->setVisible(true);
        // Update sound button text based on current state
        Button* soundButton = settingsMenu->getButtonAt(0);
        if (soundButton) {
            soundButton->setText(soundManager->isMuted() ? "Sound: OFF" : "Sound: ON");
        } });
    mainMenu->addButton("Exit", [this]()
                        { window.close(); });
    mainMenu->setPosition((windowWidth - mainMenu->getBounds().width) / 2.0f,
                          (windowHeight - mainMenu->getBounds().height) / 2.0f);

    // Initialize settings menu
    settingsMenu = std::make_unique<Menu>(*resourceManager, *soundManager);
    settingsMenu->addTitle("Settings");
    settingsMenu->addButton("Sound: ON", [this]()
                            {
        bool muted = soundManager->isMuted();
        soundManager->mute(!muted);
        Button* soundButton = settingsMenu->getButtonAt(0);
        if (soundButton) {
            soundButton->setText(muted ? "Sound: ON" : "Sound: OFF");
        } });

    // Add theme buttons for each available theme
    for (const auto &themeName : themeManager->getAvailableThemes())
    {
        settingsMenu->addButton("Theme: " + themeName, [this, themeName]()
                                {
            themeManager->setCurrentTheme(themeName);
            gameBoard->setTheme(themeName); });
    }

    settingsMenu->addButton("Back", [this]()
                            {
        currentState = GameState::MainMenu;
        settingsMenu->setVisible(false);
        mainMenu->setVisible(true); });
    settingsMenu->setPosition((windowWidth - settingsMenu->getBounds().width) / 2.0f,
                              (windowHeight - settingsMenu->getBounds().height) / 2.0f);
    settingsMenu->setVisible(false);

    // Initialize game over menu
    gameOverMenu = std::make_unique<Menu>(*resourceManager, *soundManager);
    gameOverMenu->addTitle("Game Over");
    gameOverMenu->addButton("Play Again", [this]()
                            {
        resetGame();
        gameOverMenu->setVisible(false); });
    gameOverMenu->addButton("Main Menu", [this]()
                            {
        currentState = GameState::MainMenu;
        gameOverMenu->setVisible(false);
        mainMenu->setVisible(true); });
    gameOverMenu->setPosition((windowWidth - gameOverMenu->getBounds().width) / 2.0f,
                              (windowHeight - gameOverMenu->getBounds().height) / 2.0f);
    gameOverMenu->setVisible(false);

    // Play background music
    soundManager->playMusic("background", true, 50.f);
}

Game::~Game()
{
    // Clean up resources if needed
}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        // Get mouse position for UI updates
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Handle UI events based on current state
        switch (currentState)
        {
        case GameState::MainMenu:
            mainMenu->handleEvent(event);
            break;

        case GameState::Settings:
            settingsMenu->handleEvent(event);
            break;

        case GameState::GameOver:
            gameOverMenu->handleEvent(event);
            break;

        case GameState::Playing:
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                // Only process clicks if game is active
                if (!isGameOver)
                {
                    sf::Vector2i boardPos = gameBoard->getBoardPosition(
                        static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y));

                    // Check if click is within board bounds
                    if (boardPos.x >= 0 && boardPos.x < 3 && boardPos.y >= 0 && boardPos.y < 3)
                    {
                        handlePlayerMove(boardPos.x, boardPos.y);
                    }
                }
            }
            break;
        }
    }
}

void Game::update(float deltaTime)
{
    // Get current mouse position for UI updates
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Update UI elements based on current state
    switch (currentState)
    {
    case GameState::MainMenu:
        mainMenu->update(mousePos);
        break;

    case GameState::Settings:
        settingsMenu->update(mousePos);
        break;

    case GameState::GameOver:
        gameOverMenu->update(mousePos);
        break;

    case GameState::Playing:
        gameBoard->update(deltaTime);
        break;
    }
}

void Game::render()
{
    window.clear(sf::Color(30, 30, 30)); // Dark background

    // Render game elements based on current state
    switch (currentState)
    {
    case GameState::MainMenu:
        window.draw(*mainMenu);
        break;

    case GameState::Settings:
        window.draw(*settingsMenu);
        break;

    case GameState::GameOver:
        window.draw(*gameBoard);
        window.draw(*gameOverMenu);
        break;

    case GameState::Playing:
        window.draw(*gameBoard);
        break;
    }

    window.display();
}

void Game::handlePlayerMove(int x, int y)
{
    // Try to make a move
    if (gameBoard->makeMove(x, y, currentPlayer))
    {
        // Check for game end conditions
        checkGameEnd();

        // Switch players if game continues
        if (!isGameOver)
        {
            switchPlayers();
        }
    }
}

void Game::switchPlayers()
{
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
}

void Game::checkGameEnd()
{
    // Check for winner
    winner = gameBoard->checkWinner();

    if (winner != 0)
    {
        isGameOver = true;
        gameOverMenu->setTitle(winner == 1 ? "Player X Wins!" : "Player O Wins!");
        gameOverMenu->setVisible(true);
        soundManager->playSound("win");
        currentState = GameState::GameOver;
    }
    // Check for draw
    else if (gameBoard->isBoardFull())
    {
        isGameOver = true;
        gameOverMenu->setTitle("It's a Draw!");
        gameOverMenu->setVisible(true);
        soundManager->playSound("draw");
        currentState = GameState::GameOver;
    }
}

void Game::resetGame()
{
    gameBoard->reset();
    currentPlayer = 1;
    isGameOver = false;
    winner = 0;
    currentState = GameState::Playing;
}