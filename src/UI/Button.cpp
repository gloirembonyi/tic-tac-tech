#include "UI/Button.h"

Button::Button(ResourceManager &resManager, SoundManager &sndManager)
    : resourceManager(resManager), soundManager(sndManager), currentState(ButtonState::Normal), normalColor(sf::Color(100, 100, 100)), hoverColor(sf::Color(150, 150, 150)), pressedColor(sf::Color(80, 80, 80)), disabledColor(sf::Color(70, 70, 70)), textColor(sf::Color::White), isEnabled(true)
{
    // Initialize button shape
    shape.setSize(sf::Vector2f(200.0f, 50.0f));
    shape.setFillColor(normalColor);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1.0f);

    // Initialize text
    if (resourceManager.hasFont("main"))
    {
        buttonText.setFont(resourceManager.getFont("main"));
    }
    buttonText.setString("Button");
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(textColor);

    // Center text in button
    updateText();
}

Button::~Button()
{
    // Clean up if needed
}

void Button::setPosition(float x, float y)
{
    shape.setPosition(x, y);
    updateText();
}

void Button::setSize(float width, float height)
{
    shape.setSize(sf::Vector2f(width, height));
    updateText();
}

void Button::setText(const std::string &text)
{
    buttonText.setString(text);
    updateText();
}

void Button::setTextColor(const sf::Color &color)
{
    textColor = color;
    buttonText.setFillColor(textColor);
}

void Button::setBackgroundColor(const sf::Color &color)
{
    normalColor = color;
    if (currentState == ButtonState::Normal && isEnabled)
    {
        shape.setFillColor(normalColor);
    }
}

void Button::setHoverColor(const sf::Color &color)
{
    hoverColor = color;
    if (currentState == ButtonState::Hover && isEnabled)
    {
        shape.setFillColor(hoverColor);
    }
}

void Button::setPressedColor(const sf::Color &color)
{
    pressedColor = color;
    if (currentState == ButtonState::Pressed && isEnabled)
    {
        shape.setFillColor(pressedColor);
    }
}

void Button::setOutlineColor(const sf::Color &color)
{
    shape.setOutlineColor(color);
}

void Button::setOutlineThickness(float thickness)
{
    shape.setOutlineThickness(thickness);
}

void Button::setFont(const std::string &fontName)
{
    if (resourceManager.hasFont(fontName))
    {
        buttonText.setFont(resourceManager.getFont(fontName));
        updateText();
    }
}

void Button::setCharacterSize(unsigned int size)
{
    buttonText.setCharacterSize(size);
    updateText();
}

void Button::setCallback(const std::function<void()> &callback)
{
    onClick = callback;
}

void Button::setEnabled(bool enabled)
{
    isEnabled = enabled;

    if (isEnabled)
    {
        shape.setFillColor(normalColor);
        buttonText.setFillColor(textColor);
    }
    else
    {
        shape.setFillColor(disabledColor);
        buttonText.setFillColor(sf::Color(150, 150, 150));
    }
}

sf::FloatRect Button::getBounds() const
{
    return shape.getGlobalBounds();
}

bool Button::contains(float x, float y) const
{
    return shape.getGlobalBounds().contains(x, y);
}

void Button::update(const sf::Vector2f &mousePosition)
{
    if (!isEnabled)
    {
        return;
    }

    updateButtonState(mousePosition);
}

void Button::handleEvent(const sf::Event &event)
{
    if (!isEnabled)
    {
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        if (contains(mousePos.x, mousePos.y))
        {
            currentState = ButtonState::Pressed;
            shape.setFillColor(pressedColor);

            // Play button sound
            if (soundManager.hasSound("button_click"))
            {
                soundManager.playSound("button_click");
            }
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        if (currentState == ButtonState::Pressed && contains(mousePos.x, mousePos.y))
        {
            // Execute callback
            if (onClick)
            {
                onClick();
            }
        }

        // Reset to appropriate state
        updateButtonState(mousePos);
    }
}

void Button::updateText()
{
    // Center text in button
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    buttonText.setPosition(
        shape.getPosition().x + shape.getSize().x / 2.0f,
        shape.getPosition().y + shape.getSize().y / 2.0f);
}

void Button::updateButtonState(const sf::Vector2f &mousePosition)
{
    ButtonState previousState = currentState;

    if (contains(mousePosition.x, mousePosition.y))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            currentState = ButtonState::Pressed;
            shape.setFillColor(pressedColor);
        }
        else
        {
            currentState = ButtonState::Hover;
            shape.setFillColor(hoverColor);

            // Play hover sound if state changed from normal to hover
            if (previousState == ButtonState::Normal && soundManager.hasSound("button_hover"))
            {
                soundManager.playSound("button_hover");
            }
        }
    }
    else
    {
        currentState = ButtonState::Normal;
        shape.setFillColor(normalColor);
    }
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(shape, states);
    target.draw(buttonText, states);
}