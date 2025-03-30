#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    // Texture management
    bool loadTexture(const std::string &name, const std::string &filename);
    sf::Texture &getTexture(const std::string &name);
    bool hasTexture(const std::string &name) const;

    // Font management
    bool loadFont(const std::string &name, const std::string &filename);
    sf::Font &getFont(const std::string &name);
    bool hasFont(const std::string &name) const;

    // Create sprite from texture
    std::unique_ptr<sf::Sprite> createSprite(const std::string &textureName);

    // Resource paths
    static const std::string TEXTURES_PATH;
    static const std::string FONTS_PATH;

    // Default resources
    void loadDefaultResources();

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;
};