#include "UI/AnimationManager.h"
#include <algorithm>
#include <cmath>

// Animation class implementation
Animation::Animation(float duration, const std::function<void(float)> &updateFunc,
                     const std::function<void()> &completionFunc)
    : duration(duration), currentTime(0.0f), isCompleted(false), updateFunction(updateFunc), completionFunction(completionFunc)
{
}

bool Animation::update(float deltaTime)
{
    if (isCompleted)
    {
        return true;
    }

    currentTime += deltaTime;

    // Normalize time value between 0 and 1
    float normalizedTime = std::min(currentTime / duration, 1.0f);

    // Call the update function with normalized time
    updateFunction(normalizedTime);

    // Check if animation is completed
    if (normalizedTime >= 1.0f)
    {
        isCompleted = true;

        // Call completion callback if provided
        if (completionFunction)
        {
            completionFunction();
        }

        return true;
    }

    return false;
}

void Animation::reset()
{
    currentTime = 0.0f;
    isCompleted = false;
}

// AnimationManager class implementation
AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
    clearAnimations();
}

void AnimationManager::addAnimation(const std::string &id, std::unique_ptr<Animation> animation)
{
    animations[id] = std::move(animation);
}

void AnimationManager::removeAnimation(const std::string &id)
{
    animations.erase(id);
}

void AnimationManager::clearAnimations()
{
    animations.clear();
}

void AnimationManager::createFadeIn(const std::string &id, sf::Sprite &sprite, float duration)
{
    // Create a function that updates the sprite's transparency
    auto updateFunc = [&sprite](float t)
    {
        sf::Color color = sprite.getColor();
        color.a = static_cast<sf::Uint8>(t * 255);
        sprite.setColor(color);
    };

    // Add the animation
    addAnimation(id, std::make_unique<Animation>(duration, updateFunc));
}

void AnimationManager::createFadeOut(const std::string &id, sf::Sprite &sprite, float duration)
{
    // Create a function that updates the sprite's transparency
    auto updateFunc = [&sprite](float t)
    {
        sf::Color color = sprite.getColor();
        color.a = static_cast<sf::Uint8>((1.0f - t) * 255);
        sprite.setColor(color);
    };

    // Add the animation
    addAnimation(id, std::make_unique<Animation>(duration, updateFunc));
}

void AnimationManager::createScale(const std::string &id, sf::Sprite &sprite,
                                   const sf::Vector2f &startScale, const sf::Vector2f &endScale,
                                   float duration)
{
    // Create a function that updates the sprite's scale
    auto updateFunc = [&sprite, startScale, endScale](float t)
    {
        // Use easing function for smoother animation
        float easedT = easeInOut(t);

        sf::Vector2f scale(
            startScale.x + (endScale.x - startScale.x) * easedT,
            startScale.y + (endScale.y - startScale.y) * easedT);

        sprite.setScale(scale);
    };

    // Add the animation
    addAnimation(id, std::make_unique<Animation>(duration, updateFunc));
}

void AnimationManager::createMove(const std::string &id, sf::Sprite &sprite,
                                  const sf::Vector2f &startPos, const sf::Vector2f &endPos,
                                  float duration)
{
    // Create a function that updates the sprite's position
    auto updateFunc = [&sprite, startPos, endPos](float t)
    {
        // Use easing function for smoother animation
        float easedT = easeInOut(t);

        sf::Vector2f position(
            startPos.x + (endPos.x - startPos.x) * easedT,
            startPos.y + (endPos.y - startPos.y) * easedT);

        sprite.setPosition(position);
    };

    // Add the animation
    addAnimation(id, std::make_unique<Animation>(duration, updateFunc));
}

void AnimationManager::createRotate(const std::string &id, sf::Sprite &sprite,
                                    float startAngle, float endAngle, float duration)
{
    // Create a function that updates the sprite's rotation
    auto updateFunc = [&sprite, startAngle, endAngle](float t)
    {
        // Use easing function for smoother animation
        float easedT = easeInOut(t);

        float rotation = startAngle + (endAngle - startAngle) * easedT;
        sprite.setRotation(rotation);
    };

    // Add the animation
    addAnimation(id, std::make_unique<Animation>(duration, updateFunc));
}

void AnimationManager::createBlink(const std::string &id, sf::Sprite &sprite,
                                   int blinkCount, float blinkDuration)
{
    // Create a function that updates the sprite's visibility
    auto updateFunc = [&sprite, blinkCount](float t)
    {
        // Calculate how many blinks should have occurred by now
        float cyclePosition = t * blinkCount * 2.0f;
        int cycle = static_cast<int>(cyclePosition);

        // Alternate visibility based on cycle
        bool visible = (cycle % 2 == 0);

        sf::Color color = sprite.getColor();
        color.a = visible ? 255 : 0;
        sprite.setColor(color);
    };

    // Add the animation
    addAnimation(id, std::make_unique<Animation>(blinkDuration, updateFunc));
}

void AnimationManager::update(float deltaTime)
{
    // Clear the completed animations list
    completedAnimations.clear();

    // Update all animations and collect completed ones
    for (auto &pair : animations)
    {
        const std::string &id = pair.first;
        Animation *animation = pair.second.get();

        if (animation->update(deltaTime))
        {
            completedAnimations.push_back(id);
        }
    }

    // Remove completed animations
    for (const auto &id : completedAnimations)
    {
        animations.erase(id);
    }
}

bool AnimationManager::isAnimating(const std::string &id) const
{
    return animations.find(id) != animations.end();
}

bool AnimationManager::hasActiveAnimations() const
{
    return !animations.empty();
}

// Easing functions for smoother animations
float AnimationManager::easeInOut(float t)
{
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

float AnimationManager::easeIn(float t)
{
    return t * t;
}

float AnimationManager::easeOut(float t)
{
    return t * (2.0f - t);
}