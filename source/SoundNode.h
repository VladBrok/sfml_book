#pragma once
#include "SceneNode.h"
#include "ResourceIdentifiers.h"

class SoundPlayer;


class SoundNode: public SceneNode
{
public:
    explicit                SoundNode(SoundPlayer& soundPlayer);
                            
    void                    playSound(const SoundEffects::ID sound,
                                      const sf::Vector2f& position);
    virtual unsigned int    getCategory() const;


private:
    SoundPlayer&            mSounds;
};

