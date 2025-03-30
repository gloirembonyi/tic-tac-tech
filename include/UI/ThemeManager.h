#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>

struct Theme
{
    // Board theme
    sf::Color boardBackgroundColor;
    sf::Color gridLineColor;

    // Player markers
    std::string xTextureName;
    std::string oTextureName;

    // UI theme
    sf::Color menuBackgroundColor;
    sf::Color buttonColor;
    sf::Color buttonHoverColor;
    sf::Color buttonPressedColor;
    sf::Color textColor;
    sf::Color titleColor;

    // Font
    std::string fontName;
};

class ThemeManager
{
public:
    ThemeManager();
    ~ThemeManager();

    void addTheme(const std::string &themeName, const Theme &theme);
    bool setCurrentTheme(const std::string &themeName);
    const Theme &getCurrentTheme() const;
    const Theme &getTheme(const std::string &themeName) const;

    std::vector<std::string> getAvailableThemes() const;
    bool themeExists(const std::string &themeName) const;

    // Default themes
    void initializeDefaultThemes();

private:
    std::unordered_map<std::string, Theme> themes;
    std::string currentThemeName;
};