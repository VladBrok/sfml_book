#include "SoundPlayer.h"
#include "SFML/Audio/Listener.hpp"
#include <cmath>


namespace
{
    const float LISTENER_Z = 300.f;
    const float ATTENUATION = 8.f;
    const float MIN_DISTANCE_2D = 20.f;
    const float MIN_DISTANCE_3D = std::sqrtf(MIN_DISTANCE_2D * MIN_DISTANCE_2D + LISTENER_Z * LISTENER_Z);
}


SoundPlayer::SoundPlayer()
{
    mSoundBuffers.load(SoundEffects::AlliedGunfire, "Media/Sounds/AlliedGunfire.wav");
    mSoundBuffers.load(SoundEffects::EnemyGunfire,  "Media/Sounds/EnemyGunfire.wav");
    mSoundBuffers.load(SoundEffects::Explosion1,    "Media/Sounds/Explosion1.wav");
    mSoundBuffers.load(SoundEffects::Explosion2,    "Media/Sounds/Explosion2.wav");
    mSoundBuffers.load(SoundEffects::LaunchMissile, "Media/Sounds/LaunchMissile.wav");
    mSoundBuffers.load(SoundEffects::CollectPickup, "Media/Sounds/CollectPickup.wav");
    mSoundBuffers.load(SoundEffects::Button,        "Media/Sounds/Button.wav");

    sf::Listener::setDirection(0.f, 0.f, -1.f);
}


void SoundPlayer::play(const SoundEffects::ID effect)
{
    play(effect, getListenerPosition());
}


void SoundPlayer::play(const SoundEffects::ID effect, const sf::Vector2f& position)
{
    mSounds.push_back(sf::Sound());
    sf::Sound& sound = mSounds.back();

    sound.setBuffer(mSoundBuffers.get(effect));
    sound.setPosition(position.x, -position.y, 0.f);
    sound.setAttenuation(ATTENUATION);
    sound.setMinDistance(MIN_DISTANCE_3D);

    sound.play();
}


void SoundPlayer::removeStoppedSounds()
{
    mSounds.remove_if([] (const sf::Sound& sound) 
    {
        return sound.getStatus() == sf::SoundSource::Stopped;
    });
}


void SoundPlayer::setListenerPosition(const sf::Vector2f& position)
{
    sf::Listener::setPosition(position.x, -position.y, LISTENER_Z);
}


sf::Vector2f SoundPlayer::getListenerPosition() const
{
    sf::Vector3f position(sf::Listener::getPosition());
    return sf::Vector2f(position.x, -position.y);
}

