#include "EmitterNode.h"
#include "ParticleNode.h"
#include "CommandQueue.h"


EmitterNode::EmitterNode(const Particle::Type type)
    : mType(type),
      mAccumulatedTime(sf::Time::Zero),
      mParticleSystem(nullptr)
{
}


void EmitterNode::updateCurrent(const sf::Time dt, CommandQueue& commands)
{
    if (mParticleSystem)
    {
        emitParticles(dt);
    }
    else
    {
        // Find particle node fith the same type as emitter node
        auto finder = 
        [this] (ParticleNode& container, sf::Time) 
        {
            if (container.getParticleType() == mType)
            {
                mParticleSystem = &container;
            }
        };
        Command command;
        command.category = Category::ParticleSystem;
        command.action = derivedAction<ParticleNode>(finder);

        commands.push(command);
    }
}


void EmitterNode::emitParticles(const sf::Time dt)
{
    const float EMISSION_RATE = 30.f;
    const sf::Time INTERVAL = sf::seconds(1.f) / EMISSION_RATE;

    mAccumulatedTime += dt;
    
    while (mAccumulatedTime > INTERVAL)
    {
        mParticleSystem->addParticle(getWorldPosition());
        mAccumulatedTime -= INTERVAL;
    }
}
