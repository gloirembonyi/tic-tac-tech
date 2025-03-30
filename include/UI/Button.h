#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

#include "../Resources/ResourceManager.h"
#include "../Resources/SoundManager.h"

class Button : public sf::Drawable
{
public:
    Button(ResourceManager &resourceManager, SoundManager &soundManager);
    ~Button();

    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setText(const std::string &text);
    void setTextColor(const sf::Color &color);
    void setBackgroundColor(const sf::Color &color);
    void setHoverColor(const sf::Color &color);
    void setPressedColor(const sf::Color &color);
    void setOutlineColor(const sf::Color &color);
    void setOutlineThickness(float thickness);
    void setFont(const std::string &fontName);
    void setCharacterSize(unsigned int size);
    void setCallback(const std::function<void()> &callback);
    void setEnabled(bool enabled);

    sf::FloatRect getBounds() const;
    bool contains(float x, float y) const;

    void update(const sf::Vector2f &mousePosition);
    void handleEvent(const sf::Event &event);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void updateText();
    void updateButtonState(const sf::Vector2f &mousePosition);

    // Visual components
    sf::RectangleShape shape;
    sf::Text buttonText;

    // Resources
    ResourceManager &resourceManager;
    SoundManager &soundManager;

    // Button states
    enum class ButtonState
    {
        Normal,
        Hover,
        Pressed,
        Disabled
    };

    ButtonState currentState;

    // Colors for different states
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color pressedColor;
    sf::Color disabledColor;
    sf::Color textColor;

    // Button properties
    bool isEnabled;
    std::function<void()> onClick;
};