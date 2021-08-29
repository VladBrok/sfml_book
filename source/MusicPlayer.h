#pragma once
#include "ResourceIdentifiers.h"
#include "SFML/System/NonCopyable.hpp"
#include "SFML/Audio/Music.hpp"
#include <unordered_map>
#include <string>


class MusicPlayer: private sf::NonCopyable
{
public:
                                               MusicPlayer();

    void                                       play(const Music::ID theme);
    void                                       stop();

    void                                       setPaused(const bool paused);
    void                                       setVolume(const float volume);


private:
    sf::Music                                  mMusic;
    std::unordered_map<Music::ID, std::string> mFilenames;
    float                                      mVolume;
};

