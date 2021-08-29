#include "SoundNode.h"
#include "SoundPlayer.h"


SoundNode::SoundNode(SoundPlayer& soundPlayer)
    : mSounds(soundPlayer)
{
}

                            
void SoundNode::playSound(const SoundEffects::ID sound, const sf::Vector2f& position)
{
    mSounds.play(sound, position);
}


unsigned int SoundNode::getCategory() const
{
    return Category::SoundEffect;
}