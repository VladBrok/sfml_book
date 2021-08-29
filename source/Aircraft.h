#pragma once
#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Projectile.h"
#include "Command.h"
#include "Animation.h"
#include "SFML/Graphics/Sprite.hpp"

class TextNode;


class Aircraft: public Entity
{
public:
    enum Type
    {
        Eagle,
        Raptor,
        Avenger,

        TypeCount
    };


public:
    explicit                Aircraft(const Type type, 
                                     const TextureHolder& textures,
                                     const FontHolder& fonts);
    virtual unsigned int    getCategory() const;
    float                   getMaxSpeed() const;
    virtual sf::FloatRect   getBoundingRect() const;

    virtual void            remove();
    void                    fire();
    void                    launchMissile();

    void                    collectMissiles(const int count);
    void                    increaseSpread();
    void                    increaseFireRate();

    void                    playLocalSound(CommandQueue& commands,
                                           const SoundEffects::ID effect);


private:
    virtual void            updateCurrent(const sf::Time dt, CommandQueue& commands);
    void                    updateMovementPattern(const sf::Time dt);
    void                    updateRollAnimation();
    void                    updateTexts();
    virtual void            drawCurrent(sf::RenderTarget& target, 
                                        sf::RenderStates states) const final;

    void                    checkProjectileLaunch(const sf::Time dt, CommandQueue& commands);
    void                    checkPickupDrop(CommandQueue& commands);
    void                    createBullet(SceneNode& node, const TextureHolder& textures);
    void                    createProjectile(SceneNode& node, 
                                             const TextureHolder& textures,
                                             const Projectile::Type type,
                                             const float offsetX,
                                             const float offsetY);
    void                    createPickup(SceneNode& node, const TextureHolder& textures);

    bool                    isAllied() const;
    virtual bool            isMarkedForRemoval() const;
   

private:
    Type                    mType;
    sf::Sprite              mSprite;
    TextNode*               mHealthDisplay;
    TextNode*               mMissileDisplay;
    float                   mTravelledDistance; // Distance travelled in the current direction
    std::size_t             mDirectionIndex; // Index of the current direction

    sf::Time                mFireCountdown;
    int                     mMissileAmmo;
    int                     mFireRateLevel;
    bool                    mIsFiring;
    bool                    mIsLaunchingMissile;
    int                     mSpreadLevel;

    Command                 mFireCommand;
    Command                 mMissileCommand;
    Command                 mDropPickupCommand;

    bool                    mPlayedExplosionSound;
    bool                    mSpawnedPickup;
    bool                    mShowExplosion;
    Animation               mExplosion;
};

