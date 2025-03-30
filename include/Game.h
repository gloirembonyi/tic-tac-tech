#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

#include "Board.h"
#include "UI/Menu.h"
#include "UI/ThemeManager.h"
#include "Resources/ResourceManager.h"
#include "Resources/SoundManager.h"

enum class GameState
{
    MainMenu,
    Playing,
    GameOver,
    Settings
};

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void handlePlayerMove(int x, int y);
    void switchPlayers();
    void checkGameEnd();
    void resetGame();

    // Core game components
    sf::RenderWindow window;
    sf::Clock gameClock;
    GameState currentState;
    std::unique_ptr<Board> gameBoard;

    // UI and resources
    std::unique_ptr<Menu> mainMenu;
    std::unique_ptr<Menu> gameOverMenu;
    std::unique_ptr<Menu> settingsMenu;
    std::unique_ptr<ThemeManager> themeManager;
    std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<SoundManager> soundManager;

    // Game state
    int currentPlayer; // 1 for X, 2 for O
    bool isGameOver;
    int winner; // 0 for draw, 1 for player 1, 2 for player 2

    // Window settings
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;
    const std::string windowTitle = "TicTacTech";
};