#pragma once
#include "SceneNode.h"


class Entity: public SceneNode
{
public:
    explicit               Entity(const int hitpoints);

    void                   repair(const int points);
    void                   damage(const int points);
    void                   destroy();
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

