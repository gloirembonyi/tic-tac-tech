#include "Resources/SoundManager.h"
#include <iostream>

// Define static constants
const std::string SoundManager::SOUNDS_PATH = "resources/sounds/";

SoundManager::SoundManager()
    : soundVolume(100.0f), musicVolume(80.0f), muted(false), currentMusic("")
{
    // Load default sounds on initialization
    loadDefaultSounds();
}

SoundManager::~SoundManager()
{
    // Stop all sounds and music
    for (auto &pair : sounds)
    {
        pair.second->stop();
    }

    stopMusic();
}

bool SoundManager::loadSound(const std::string &name, const std::string &filename)
{
    // Create a new sound buffer
    auto buffer = std::make_unique<sf::SoundBuffer>();

    // Try to load the sound
    if (!buffer->loadFromFile(SOUNDS_PATH + filename))
    {
        std::cerr << "Failed to load sound: " << filename << std::endl;
        // Don't return false here, we'll create an empty sound instead
        // to avoid crashes when the sound is played

        // Create a silent sound buffer
        const int sampleRate = 44100;
        const int duration = 1; // 1 second
        std::vector<sf::Int16> samples(sampleRate * duration, 0);

        // Load silent buffer
        if (!buffer->loadFromSamples(samples.data(), samples.size(), 1, sampleRate))
        {
            std::cerr << "Failed to create silent sound buffer for: " << name << std::endl;
            return false;
        }

        std::cout << "Created silent fallback sound for: " << filename << std::endl;
    }

    // Create a sound with the buffer
    auto sound = std::make_unique<sf::Sound>();
    sound->setBuffer(*buffer);
    sound->setVolume(soundVolume);

    // Store the buffer and sound
    soundBuffers[name] = std::move(buffer);
    sounds[name] = std::move(sound);

    return true;
}

void SoundManager::playSound(const std::string &name, float volume)
{
    // Check if sound exists
    if (!hasSound(name))
    {
        std::cerr << "Sound not found: " << name << std::endl;
        return;
    }

    // Don't play if muted
    if (muted)
    {
        return;
    }

    // Set volume and play the sound
    sounds[name]->setVolume(volume);
    sounds[name]->play();
}

void SoundManager::stopSound(const std::string &name)
{
    // Check if sound exists
    if (!hasSound(name))
    {
        std::cerr << "Sound not found: " << name << std::endl;
        return;
    }

    sounds[name]->stop();
}

bool SoundManager::hasSound(const std::string &name) const
{
    return sounds.find(name) != sounds.end();
}

bool SoundManager::loadMusic(const std::string &name, const std::string &filename)
{
    // Create a new music instance
    auto newMusic = std::make_unique<sf::Music>();

    // Try to load the music
    if (!newMusic->openFromFile(SOUNDS_PATH + filename))
    {
        std::cerr << "Failed to load music: " << filename << std::endl;
        // We'll return false but not crash the game - music is non-essential
        return false;
    }

    // Set volume
    newMusic->setVolume(musicVolume);

    // Store the music
    music[name] = std::move(newMusic);

    return true;
}

void SoundManager::playMusic(const std::string &name, bool loop, float volume)
{
    // Check if music exists
    if (music.find(name) == music.end())
    {
        // Silently fail if music doesn't exist
        std::cerr << "Music not found: " << name << std::endl;
        return;
    }

    // Stop current music if different
    if (currentMusic != "" && currentMusic != name)
    {
        stopMusic();
    }

    // Don't play if muted
    if (muted)
    {
        currentMusic = name;
        return;
    }

    // Set properties and play
    music[name]->setLoop(loop);
    music[name]->setVolume(volume);

    if (music[name]->getStatus() != sf::Music::Playing)
    {
        music[name]->play();
    }

    currentMusic = name;
}

void SoundManager::stopMusic()
{
    if (currentMusic != "" && music.find(currentMusic) != music.end())
    {
        music[currentMusic]->stop();
    }
    currentMusic = "";
}

void SoundManager::pauseMusic()
{
    if (currentMusic != "" && music.find(currentMusic) != music.end())
    {
        if (music[currentMusic]->getStatus() == sf::Music::Playing)
        {
            music[currentMusic]->pause();
        }
    }
}

void SoundManager::resumeMusic()
{
    if (currentMusic != "" && music.find(currentMusic) != music.end())
    {
        if (music[currentMusic]->getStatus() == sf::Music::Paused)
        {
            music[currentMusic]->play();
        }
    }
}

bool SoundManager::isMusicPlaying() const
{
    if (currentMusic != "" && music.find(currentMusic) != music.end())
    {
        return music.at(currentMusic)->getStatus() == sf::Music::Playing;
    }
    return false;
}

void SoundManager::setSoundVolume(float volume)
{
    // Clamp volume between 0 and 100
    soundVolume = std::min(100.0f, std::max(0.0f, volume));

    // Apply to all sounds
    for (auto &pair : sounds)
    {
        pair.second->setVolume(soundVolume);
    }
}

void SoundManager::setMusicVolume(float volume)
{
    // Clamp volume between 0 and 100
    musicVolume = std::min(100.0f, std::max(0.0f, volume));

    // Apply to all music
    for (auto &pair : music)
    {
        pair.second->setVolume(musicVolume);
    }
}

float SoundManager::getSoundVolume() const
{
    return soundVolume;
}

float SoundManager::getMusicVolume() const
{
    return musicVolume;
}

void SoundManager::mute(bool muteState)
{
    muted = muteState;

    if (muted)
    {
        // Pause current music
        pauseMusic();
    }
    else
    {
        // Resume current music
        resumeMusic();
    }
}

bool SoundManager::isMuted() const
{
    return muted;
}

void SoundManager::loadDefaultSounds()
{
    // Load sound effects
    loadSound("button_click", "button_click.wav");
    loadSound("button_hover", "button_hover.wav");
    loadSound("x_move", "x_move.wav");
    loadSound("o_move", "o_move.wav");
    loadSound("win", "win.wav");
    loadSound("draw", "draw.wav");

    // Load music
    loadMusic("background", "background_music.ogg");
    loadMusic("menu", "menu_music.ogg");
}