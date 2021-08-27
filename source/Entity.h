#pragma once
#include "SceneNode.h"


class Entity: public SceneNode
{
public:
    explicit               Entity(const int hitpoints);

    void                   repair(const int points);
    void                   damage(const int points);

    // Call when the entity is destroyed by another entity
    void                   destroy();

    // Call when the entity is ouside the visible area
    virtual void           remove();

    virtual bool           isDestroyed() const;
    int                    getHitpoints() const;

    void                   accelerate(const sf::Vector2f& velocity);
    void                   accelerate(const float x, const float y);
    void                   setVelocity(const sf::Vector2f& velocity);
    void                   setVelocity(const float x, const float y);
    const sf::Vector2f&    getVelocity() const;


protected:
    virtual void           updateCurrent(const sf::Time dt, CommandQueue& commands);


private:
    int                    mHitpoints;
    sf::Vector2f           mVelocity; 
};

