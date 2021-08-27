#include "Entity.h"
#include "CommandQueue.h"


Entity::Entity(const int hitpoints)
    : mHitpoints(hitpoints)
{
}
     

void Entity::repair(const int points)
{
    mHitpoints += points;
}


void Entity::damage(const int points)
{
    mHitpoints -= points;
}


void Entity::destroy()
{
    mHitpoints = 0;
}


void Entity::remove()
{
    destroy();
}


bool Entity::isDestroyed() const
{
    return mHitpoints <= 0;
}


int  Entity::getHitpoints() const
{
    return mHitpoints;
}


void Entity::setVelocity(const sf::Vector2f& velocity)
{
    mVelocity = velocity;
}


void Entity::accelerate(const sf::Vector2f& velocity)
{
    mVelocity += velocity;
}


void Entity::accelerate(const float x, const float y)
{
    mVelocity.x += x;
    mVelocity.y += y;
}


void Entity::setVelocity(const float x, const float y)
{
    mVelocity.x = x;
    mVelocity.y = y;
}


const sf::Vector2f& Entity::getVelocity() const
{
    return mVelocity;
}


void Entity::updateCurrent(const sf::Time dt, CommandQueue&)
{
    move(mVelocity * dt.asSeconds());
}

