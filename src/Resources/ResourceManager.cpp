#include "Resources/ResourceManager.h"
#include <iostream>

// Define static constants
const std::string ResourceManager::TEXTURES_PATH = "resources/images/";
const std::string ResourceManager::FONTS_PATH = "resources/fonts/";

ResourceManager::ResourceManager()
{
    // Load default resources on initialization
    loadDefaultResources();
}

ResourceManager::~ResourceManager()
{
    // Resources will be automatically cleaned up
}

bool ResourceManager::loadTexture(const std::string &name, const std::string &filename)
{
    // Create a new texture
    std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();

    // Try to load the texture
    if (!texture->loadFromFile(TEXTURES_PATH + filename))
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;

        // For SVG files, if a PNG version isn't found, attempt to create a fallback
        if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".svg")
        {
            std::string fallbackFilename = filename.substr(0, filename.size() - 4) + ".png";

            // Create a simple fallback texture if SVG can't be loaded directly
            sf::RenderTexture renderTexture;
            renderTexture.create(100, 100);
            renderTexture.clear(sf::Color(50, 50, 50, 0)); // Transparent background

            // Draw different fallbacks based on the name
            if (name.find("x_marker") != std::string::npos)
            {
                // Draw an X
                sf::RectangleShape line1(sf::Vector2f(80, 10));
                line1.setOrigin(40, 5);
                line1.setPosition(50, 50);
                line1.setRotation(45);
                line1.setFillColor(sf::Color::Red);

                sf::RectangleShape line2(sf::Vector2f(80, 10));
                line2.setOrigin(40, 5);
                line2.setPosition(50, 50);
                line2.setRotation(-45);
                line2.setFillColor(sf::Color::Red);

                renderTexture.draw(line1);
                renderTexture.draw(line2);
            }
            else if (name.find("o_marker") != std::string::npos)
            {
                // Draw an O
                sf::CircleShape circle(35);
                circle.setOrigin(35, 35);
                circle.setPosition(50, 50);
                circle.setFillColor(sf::Color::Transparent);
                circle.setOutlineColor(sf::Color::Blue);
                circle.setOutlineThickness(10);

                renderTexture.draw(circle);
            }
            else if (name.find("button") != std::string::npos)
            {
                // Draw a button
                sf::RectangleShape button(sf::Vector2f(90, 30));
                button.setOrigin(45, 15);
                button.setPosition(50, 50);
                button.setFillColor(sf::Color(100, 100, 100));
                button.setOutlineColor(sf::Color::White);
                button.setOutlineThickness(2);

                renderTexture.draw(button);
            }

            renderTexture.display();

            // Try to use the fallback texture
            texture = std::make_unique<sf::Texture>(renderTexture.getTexture());
            std::cout << "Created fallback texture for: " << filename << std::endl;
        }
        else
        {
            return false;
        }
    }

    // Store the texture
    textures[name] = std::move(texture);
    return true;
}

sf::Texture &ResourceManager::getTexture(const std::string &name)
{
    // Check if texture exists
    if (!hasTexture(name))
    {
        throw std::runtime_error("Texture not found: " + name);
    }

    return *textures[name];
}

bool ResourceManager::hasTexture(const std::string &name) const
{
    return textures.find(name) != textures.end();
}

bool ResourceManager::loadFont(const std::string &name, const std::string &filename)
{
    // Create a new font
    std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();

    // Try to load the font
    if (!font->loadFromFile(FONTS_PATH + filename))
    {
        std::cerr << "Failed to load font: " << filename << std::endl;
        return false;
    }

    // Store the font
    fonts[name] = std::move(font);
    return true;
}

sf::Font &ResourceManager::getFont(const std::string &name)
{
    // Check if font exists
    if (!hasFont(name))
    {
        throw std::runtime_error("Font not found: " + name);
    }

    return *fonts[name];
}

bool ResourceManager::hasFont(const std::string &name) const
{
    return fonts.find(name) != fonts.end();
}

std::unique_ptr<sf::Sprite> ResourceManager::createSprite(const std::string &textureName)
{
    // Check if texture exists
    if (!hasTexture(textureName))
    {
        throw std::runtime_error("Cannot create sprite. Texture not found: " + textureName);
    }

    // Create and return a new sprite with the specified texture
    auto sprite = std::make_unique<sf::Sprite>();
    sprite->setTexture(getTexture(textureName));

    return sprite;
}

void ResourceManager::loadDefaultResources()
{
    // Load default fonts
    loadFont("main", "Open-Sans-Regular.ttf");
    loadFont("title", "Open-Sans-Bold.ttf");

    // Load default textures for game markers
    loadTexture("x_marker", "x.svg");
    loadTexture("o_marker", "o.svg");
    loadTexture("x_marker_dark", "x_dark.svg");
    loadTexture("o_marker_dark", "o_dark.svg");
    loadTexture("x_marker_color", "x_color.svg");
    loadTexture("o_marker_color", "o_color.svg");

    // Load UI textures
    loadTexture("background", "background.svg");
    loadTexture("button", "button.svg");
    loadTexture("button_hover", "button_hover.svg");
    loadTexture("button_pressed", "button_pressed.svg");
}