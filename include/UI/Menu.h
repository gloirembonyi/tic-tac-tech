#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include <functional>

#include "Button.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/SoundManager.h"

class Menu : public sf::Drawable
{
public:
    Menu(ResourceManager &resourceManager, SoundManager &soundManager);
    ~Menu();

    void addButton(const std::string &text, const std::function<void()> &callback);
    void addTitle(const std::string &text);
    void setTitle(const std::string &text);
    void setPosition(float x, float y);
    void setSpacing(float spacing);
    void setBackgroundColor(const sf::Color &color);
    void setFont(const std::string &fontName);
    void setTitleSize(unsigned int size);
    void setTitleColor(const sf::Color &color);
    void setButtonSize(float width, float height);
    void setVisible(bool visible);

    bool isVisible() const;
    sf::FloatRect getBounds() const;

    void update(const sf::Vector2f &mousePosition);
    void handleEvent(const sf::Event &event);

    // Public access to buttons for direct manipulation
    std::vector<std::unique_ptr<Button>> &getButtons() { return buttons; }
    Button *getButtonAt(size_t index);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void updateLayout();

    // Visual components
    sf::RectangleShape background;
    sf::Text titleText;
    std::vector<std::unique_ptr<Button>> buttons;

    // Resources
    ResourceManager &resourceManager;
    SoundManager &soundManager;

    // Menu properties
    sf::Vector2f position;
    float buttonSpacing;
    float padding;
    bool visible;
};