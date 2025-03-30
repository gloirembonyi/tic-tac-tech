#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <memory>

class Animation
{
public:
    Animation(float duration, const std::function<void(float)> &updateFunc,
              const std::function<void()> &completionFunc = nullptr);

    bool update(float deltaTime);
    void reset();

private:
    float duration;
    float currentTime;
    bool isCompleted;
    std::function<void(float)> updateFunction;
    std::function<void()> completionFunction;
};

class AnimationManager
{
public:
    AnimationManager();
    ~AnimationManager();

    // Animation creation
    void addAnimation(const std::string &id, std::unique_ptr<Animation> animation);
    void removeAnimation(const std::string &id);
    void clearAnimations();

    // Predefined animations
    void createFadeIn(const std::string &id, sf::Sprite &sprite, float duration);
    void createFadeOut(const std::string &id, sf::Sprite &sprite, float duration);
    void createScale(const std::string &id, sf::Sprite &sprite,
                     const sf::Vector2f &startScale, const sf::Vector2f &endScale, float duration);
    void createMove(const std::string &id, sf::Sprite &sprite,
                    const sf::Vector2f &startPos, const sf::Vector2f &endPos, float duration);
    void createRotate(const std::string &id, sf::Sprite &sprite,
                      float startAngle, float endAngle, float duration);
    void createBlink(const std::string &id, sf::Sprite &sprite,
                     int blinkCount, float blinkDuration);

    // Animation control
    void update(float deltaTime);
    bool isAnimating(const std::string &id) const;
    bool hasActiveAnimations() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Animation>> animations;
    std::vector<std::string> completedAnimations;

    // Helper functions
    float easeInOut(float t);
    float easeIn(float t);
    float easeOut(float t);
};