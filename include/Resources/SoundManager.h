#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    // Sound effects
    bool loadSound(const std::string &name, const std::string &filename);
    void playSound(const std::string &name, float volume = 100.f);
    void stopSound(const std::string &name);
    bool hasSound(const std::string &name) const;

    // Music
    bool loadMusic(const std::string &name, const std::string &filename);
    void playMusic(const std::string &name, bool loop = true, float volume = 100.f);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    bool isMusicPlaying() const;

    // Volume control
    void setSoundVolume(float volume);
    void setMusicVolume(float volume);
    float getSoundVolume() const;
    float getMusicVolume() const;
    void mute(bool mute);
    bool isMuted() const;

    // Resource paths
    static const std::string SOUNDS_PATH;

    // Default resources
    void loadDefaultSounds();

private:
    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> sounds;
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> music;

    std::string currentMusic;
    float soundVolume;
    float musicVolume;
    bool muted;
};