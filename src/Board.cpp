#include "Board.h"
#include <iostream>

Board::Board(ResourceManager &resManager, SoundManager &sndManager)
    : resourceManager(resManager), soundManager(sndManager), cellSize(100.0f), position(0.0f, 0.0f), gridColor(sf::Color::White), backgroundColor(sf::Color(50, 50, 50)), currentTheme("default")
{
    // Initialize animation manager
    animationManager = std::make_unique<AnimationManager>();

    // Set up the board
    initializeBoard();
    createGridLines();
}

Board::~Board()
{
    // Clean up if needed
}

void Board::initializeBoard()
{
    // Initialize the grid with all cells empty (0)
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            grid[i][j] = 0;
        }
    }

    // Create board background
    boardBackground.setSize(sf::Vector2f(cellSize * 3, cellSize * 3));
    boardBackground.setFillColor(backgroundColor);
    boardBackground.setPosition(position);

    // Initialize cell sprites
    cellSprites.clear();
    for (int i = 0; i < 9; ++i)
    {
        cellSprites.push_back(nullptr);
    }
}

void Board::createGridLines()
{
    gridLines.clear();

    // Create horizontal lines
    for (int i = 1; i < 3; ++i)
    {
        sf::RectangleShape line;
        line.setSize(sf::Vector2f(cellSize * 3, 2.0f));
        line.setFillColor(gridColor);
        line.setPosition(position.x, position.y + i * cellSize);
        gridLines.push_back(line);
    }

    // Create vertical lines
    for (int i = 1; i < 3; ++i)
    {
        sf::RectangleShape line;
        line.setSize(sf::Vector2f(2.0f, cellSize * 3));
        line.setFillColor(gridColor);
        line.setPosition(position.x + i * cellSize, position.y);
        gridLines.push_back(line);
    }
}

bool Board::makeMove(int x, int y, int player)
{
    // Check if the move is valid
    if (x < 0 || x >= 3 || y < 0 || y >= 3 || grid[y][x] != 0)
    {
        return false;
    }

    // Make the move
    grid[y][x] = player;

    // Create and position the player marker sprite
    int cellIndex = y * 3 + x;
    std::string textureName = (player == 1) ? "x_marker" : "o_marker";

    cellSprites[cellIndex] = resourceManager.createSprite(textureName);
    cellSprites[cellIndex]->setPosition(
        position.x + x * cellSize + (cellSize - cellSprites[cellIndex]->getGlobalBounds().width) / 2.0f,
        position.y + y * cellSize + (cellSize - cellSprites[cellIndex]->getGlobalBounds().height) / 2.0f);

    // Set initial scale to 0 for grow animation
    cellSprites[cellIndex]->setScale(0.0f, 0.0f);

    // Add animation for the marker
    triggerMoveAnimation(x, y, player);

    // Play sound effect
    playMoveSound(player);

    return true;
}

void Board::triggerMoveAnimation(int x, int y, int player)
{
    int cellIndex = y * 3 + x;
    std::string animId = "cell_" + std::to_string(cellIndex);

    // Scale animation (grow from 0 to 1)
    animationManager->createScale(
        animId,
        *cellSprites[cellIndex],
        sf::Vector2f(0.0f, 0.0f),
        sf::Vector2f(1.0f, 1.0f),
        0.2f);
}

void Board::playMoveSound(int player)
{
    std::string soundName = (player == 1) ? "x_move" : "o_move";
    soundManager.playSound(soundName);
}

bool Board::isCellEmpty(int x, int y) const
{
    if (x < 0 || x >= 3 || y < 0 || y >= 3)
    {
        return false;
    }
    return grid[y][x] == 0;
}

int Board::checkWinner() const
{
    // Check rows
    for (int i = 0; i < 3; ++i)
    {
        if (grid[i][0] != 0 && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2])
        {
            return grid[i][0];
        }
    }

    // Check columns
    for (int i = 0; i < 3; ++i)
    {
        if (grid[0][i] != 0 && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i])
        {
            return grid[0][i];
        }
    }

    // Check diagonals
    if (grid[0][0] != 0 && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
    {
        return grid[0][0];
    }

    if (grid[0][2] != 0 && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0])
    {
        return grid[0][2];
    }

    return 0; // No winner
}

bool Board::isBoardFull() const
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (grid[i][j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

void Board::reset()
{
    // Clear the grid
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            grid[i][j] = 0;
        }
    }

    // Clear all cell sprites
    for (auto &sprite : cellSprites)
    {
        sprite = nullptr;
    }

    // Clear all animations
    animationManager->clearAnimations();
}

sf::Vector2i Board::getBoardPosition(float x, float y) const
{
    // Convert screen coordinates to board grid coordinates
    sf::Vector2i boardPos(-1, -1);

    if (x >= position.x && x < position.x + cellSize * 3 &&
        y >= position.y && y < position.y + cellSize * 3)
    {
        boardPos.x = static_cast<int>((x - position.x) / cellSize);
        boardPos.y = static_cast<int>((y - position.y) / cellSize);
    }

    return boardPos;
}

void Board::setPosition(float x, float y)
{
    position = sf::Vector2f(x, y);

    // Update background position
    boardBackground.setPosition(position);

    // Update grid lines
    createGridLines();
}

sf::FloatRect Board::getBounds() const
{
    return sf::FloatRect(position.x, position.y, cellSize * 3, cellSize * 3);
}

void Board::update(float deltaTime)
{
    // Update animations
    animationManager->update(deltaTime);
}

void Board::setTheme(const std::string &themeName)
{
    currentTheme = themeName;

    // Update visual elements based on theme
    // In a real implementation, this would use the theme manager
    // to get colors and textures for the current theme
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Draw board background
    target.draw(boardBackground, states);

    // Draw grid lines
    for (const auto &line : gridLines)
    {
        target.draw(line, states);
    }

    // Draw cell sprites
    for (const auto &sprite : cellSprites)
    {
        if (sprite != nullptr)
        {
            target.draw(*sprite, states);
        }
    }
}