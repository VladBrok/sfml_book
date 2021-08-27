#pragma once
#include "SceneNode.h"
#include "Particle.h"

class ParticleNode;


class EmitterNode: public SceneNode
{
public:
    explicit          EmitterNode(const Particle::Type type);


private:
    virtual void      updateCurrent(const sf::Time dt, CommandQueue& commands);                    
    void              emitParticles(const sf::Time dt);


private:
    sf::Time          mAccumulatedTime;
    Particle::Type    mType;
    ParticleNode*     mParticleSystem;
};

