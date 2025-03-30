#include "UI/Menu.h"

Menu::Menu(ResourceManager &resManager, SoundManager &sndManager)
    : resourceManager(resManager), soundManager(sndManager), position(0.0f, 0.0f), buttonSpacing(10.0f), padding(20.0f), visible(true)
{
    // Initialize background
    background.setFillColor(sf::Color(40, 40, 40, 230));

    // Initialize title text
    if (resourceManager.hasFont("main"))
    {
        titleText.setFont(resourceManager.getFont("main"));
    }
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("Menu");

    // Position elements
    updateLayout();
}

Menu::~Menu()
{
    // Clean up if needed
}

void Menu::addButton(const std::string &text, const std::function<void()> &callback)
{
    auto button = std::make_unique<Button>(resourceManager, soundManager);
    button->setText(text);
    button->setCallback(callback);
    button->setSize(200.0f, 50.0f);

    buttons.push_back(std::move(button));
    updateLayout();
}

void Menu::addTitle(const std::string &text)
{
    titleText.setString(text);
    updateLayout();
}

void Menu::setTitle(const std::string &text)
{
    titleText.setString(text);
    updateLayout();
}

void Menu::setPosition(float x, float y)
{
    position = sf::Vector2f(x, y);
    updateLayout();
}

void Menu::setSpacing(float spacing)
{
    buttonSpacing = spacing;
    updateLayout();
}

void Menu::setBackgroundColor(const sf::Color &color)
{
    background.setFillColor(color);
}

void Menu::setFont(const std::string &fontName)
{
    if (resourceManager.hasFont(fontName))
    {
        titleText.setFont(resourceManager.getFont(fontName));

        for (auto &button : buttons)
        {
            button->setFont(fontName);
        }

        updateLayout();
    }
}

void Menu::setTitleSize(unsigned int size)
{
    titleText.setCharacterSize(size);
    updateLayout();
}

void Menu::setTitleColor(const sf::Color &color)
{
    titleText.setFillColor(color);
}

void Menu::setButtonSize(float width, float height)
{
    for (auto &button : buttons)
    {
        button->setSize(width, height);
    }
    updateLayout();
}

void Menu::setVisible(bool isVisible)
{
    visible = isVisible;
}

bool Menu::isVisible() const
{
    return visible;
}

sf::FloatRect Menu::getBounds() const
{
    return background.getGlobalBounds();
}

void Menu::update(const sf::Vector2f &mousePosition)
{
    if (!visible)
    {
        return;
    }

    for (auto &button : buttons)
    {
        button->update(mousePosition);
    }
}

void Menu::handleEvent(const sf::Event &event)
{
    if (!visible)
    {
        return;
    }

    for (auto &button : buttons)
    {
        button->handleEvent(event);
    }
}

Button *Menu::getButtonAt(size_t index)
{
    if (index < buttons.size())
    {
        return buttons[index].get();
    }
    return nullptr;
}

void Menu::updateLayout()
{
    // Calculate menu dimensions based on content
    float menuWidth = 0.0f;
    float menuHeight = 0.0f;

    // Title height
    float titleHeight = titleText.getLocalBounds().height + padding * 2;

    // Find the widest button
    for (const auto &button : buttons)
    {
        menuWidth = std::max(menuWidth, button->getBounds().width);
    }

    // Add padding to width
    menuWidth += padding * 2;

    // Set title position
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top);
    titleText.setPosition(position.x + menuWidth / 2.0f, position.y + padding);

    // Calculate total height and set button positions
    menuHeight = titleHeight;
    for (size_t i = 0; i < buttons.size(); ++i)
    {
        buttons[i]->setPosition(
            position.x + (menuWidth - buttons[i]->getBounds().width) / 2.0f,
            position.y + menuHeight + padding);
        menuHeight += buttons[i]->getBounds().height + buttonSpacing;
    }

    // Add bottom padding
    menuHeight += padding;

    // Set background size and position
    background.setSize(sf::Vector2f(menuWidth, menuHeight));
    background.setPosition(position);
}

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!visible)
    {
        return;
    }

    target.draw(background, states);
    target.draw(titleText, states);

    for (const auto &button : buttons)
    {
        target.draw(*button, states);
    }
}