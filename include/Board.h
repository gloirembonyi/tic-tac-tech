#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <memory>

#include "UI/AnimationManager.h"
#include "Resources/ResourceManager.h"
#include "Resources/SoundManager.h"

class Board : public sf::Drawable {
public:
    Board(ResourceManager& resourceManager, SoundManager& soundManager);
    ~Board();

    // Game logic
    bool makeMove(int x, int y, int player);
    bool isCellEmpty(int x, int y) const;
    int checkWinner() const; // 0 for no winner, 1 for player 1, 2 for player 2
    bool isBoardFull() const;
    void reset();
    
    // UI interaction
    sf::Vector2i getBoardPosition(float x, float y) const;
    void setPosition(float x, float y);
    sf::FloatRect getBounds() const;
    
    // Graphics
    void update(float deltaTime);
    void setTheme(const std::string& themeName);
    
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    void initializeBoard();
    void createGridLines();
    void playMoveSound(int player);
    void triggerMoveAnimation(int x, int y, int player);
    
    // Game data
    std::array<std::array<int, 3>, 3> grid; // 0 for empty, 1 for X, 2 for O
    
    // Graphics components
    sf::RectangleShape boardBackground;
    std::vector<sf::RectangleShape> gridLines;
    std::vector<std::unique_ptr<sf::Sprite>> cellSprites;
    
    // Resources and effects
    ResourceManager& resourceManager;
    SoundManager& soundManager;
    std::unique_ptr<AnimationManager> animationManager;
    
    // Board properties
    float cellSize;
    sf::Vector2f position;
    sf::Color gridColor;
    sf::Color backgroundColor;
    
    // Current theme
    std::string currentTheme;
}; 