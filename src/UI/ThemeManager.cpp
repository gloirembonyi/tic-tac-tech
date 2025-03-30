#include "UI/ThemeManager.h"
#include <stdexcept>

ThemeManager::ThemeManager()
    : currentThemeName("default")
{
    initializeDefaultThemes();
}

ThemeManager::~ThemeManager()
{
    // Clean up if needed
}

void ThemeManager::addTheme(const std::string &themeName, const Theme &theme)
{
    themes[themeName] = theme;
}

bool ThemeManager::setCurrentTheme(const std::string &themeName)
{
    if (themeExists(themeName))
    {
        currentThemeName = themeName;
        return true;
    }
    return false;
}

const Theme &ThemeManager::getCurrentTheme() const
{
    return themes.at(currentThemeName);
}

const Theme &ThemeManager::getTheme(const std::string &themeName) const
{
    if (!themeExists(themeName))
    {
        throw std::runtime_error("Theme does not exist: " + themeName);
    }
    return themes.at(themeName);
}

std::vector<std::string> ThemeManager::getAvailableThemes() const
{
    std::vector<std::string> themeNames;
    for (const auto &pair : themes)
    {
        themeNames.push_back(pair.first);
    }
    return themeNames;
}

bool ThemeManager::themeExists(const std::string &themeName) const
{
    return themes.find(themeName) != themes.end();
}

void ThemeManager::initializeDefaultThemes()
{
    // Default theme (Classic)
    Theme defaultTheme;
    defaultTheme.boardBackgroundColor = sf::Color(50, 50, 50);
    defaultTheme.gridLineColor = sf::Color::White;
    defaultTheme.xTextureName = "x_marker";
    defaultTheme.oTextureName = "o_marker";
    defaultTheme.menuBackgroundColor = sf::Color(40, 40, 40, 230);
    defaultTheme.buttonColor = sf::Color(100, 100, 100);
    defaultTheme.buttonHoverColor = sf::Color(150, 150, 150);
    defaultTheme.buttonPressedColor = sf::Color(80, 80, 80);
    defaultTheme.textColor = sf::Color::White;
    defaultTheme.titleColor = sf::Color::White;
    defaultTheme.fontName = "main";

    // Dark theme
    Theme darkTheme;
    darkTheme.boardBackgroundColor = sf::Color(20, 20, 20);
    darkTheme.gridLineColor = sf::Color(100, 100, 100);
    darkTheme.xTextureName = "x_marker_dark";
    darkTheme.oTextureName = "o_marker_dark";
    darkTheme.menuBackgroundColor = sf::Color(20, 20, 20, 230);
    darkTheme.buttonColor = sf::Color(40, 40, 40);
    darkTheme.buttonHoverColor = sf::Color(60, 60, 60);
    darkTheme.buttonPressedColor = sf::Color(30, 30, 30);
    darkTheme.textColor = sf::Color(200, 200, 200);
    darkTheme.titleColor = sf::Color(220, 220, 220);
    darkTheme.fontName = "main";

    // Colorful theme
    Theme colorfulTheme;
    colorfulTheme.boardBackgroundColor = sf::Color(40, 60, 80);
    colorfulTheme.gridLineColor = sf::Color(220, 220, 100);
    colorfulTheme.xTextureName = "x_marker_color";
    colorfulTheme.oTextureName = "o_marker_color";
    colorfulTheme.menuBackgroundColor = sf::Color(40, 60, 80, 230);
    colorfulTheme.buttonColor = sf::Color(60, 120, 180);
    colorfulTheme.buttonHoverColor = sf::Color(80, 140, 200);
    colorfulTheme.buttonPressedColor = sf::Color(40, 100, 160);
    colorfulTheme.textColor = sf::Color(255, 255, 180);
    colorfulTheme.titleColor = sf::Color(255, 200, 100);
    colorfulTheme.fontName = "main";

    // Add themes
    addTheme("default", defaultTheme);
    addTheme("dark", darkTheme);
    addTheme("colorful", colorfulTheme);

    // Set default theme
    currentThemeName = "default";
}