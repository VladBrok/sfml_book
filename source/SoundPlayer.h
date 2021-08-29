#pragma once
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

#include "SFML/System/Vector2.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Audio/Sound.hpp"

#include <list>


class SoundPlayer
{
public:
                            SoundPlayer();

    void                    play(const SoundEffects::ID effect);
    void                    play(const SoundEffects::ID effect, const sf::Vector2f& position);
                            
    void                    removeStoppedSounds();
                            
    void                    setListenerPosition(const sf::Vector2f& position);
    sf::Vector2f            getListenerPosition() const;


private:
    SoundBufferHolder       mSoundBuffers;
    std::list<sf::Sound>    mSounds;
};

