#pragma once
#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "SFML/Graphics/Sprite.hpp"
class CommandQueue;


class Projectile: public Entity
{
public:
    enum Type
    {
        AlliedBullet,
        EnemyBullet,
        Missile,

        TypeCount
    };


public:
                             Projectile(const Type type, const TextureHolder& textures);
    
    void                     guideTowards(const sf::Vector2f& position);
    bool                     isGuided() const;
                             
    virtual unsigned int     getCategory() const;
    virtual sf::FloatRect    getBoundingRect() const;
    float                    getMaxSpeed() const;
    int	                     getDamage() const;

                             
private:                     
    virtual void             updateCurrent(const sf::Time dt, CommandQueue& commands);
    virtual void             drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
    Type                     mType;
    sf::Sprite               mSprite;
    sf::Vector2f             mTargetDirection;
};

