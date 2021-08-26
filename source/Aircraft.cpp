#include "Aircraft.h"
#include "ResourceHolder.h"
#include "DataTables.h"
#include "Utility.h"
#include "TextNode.h"
#include "CommandQueue.h"
#include "Pickup.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include <iostream>


namespace
{
    const std::vector<AircraftData> DATA_TABLE(initAircraftData());
}


Aircraft::Aircraft(const Type type, const TextureHolder& textures, const FontHolder& fonts)
    : Entity(DATA_TABLE[type].hitpoints),
      mType(type), 
      mSprite(textures.get(DATA_TABLE[type].textureId), DATA_TABLE[type].textureRect),
      mHealthDisplay(nullptr),
      mMissileDisplay(nullptr),
      mTravelledDistance(0.f),
      mDirectionIndex(0),
      mFireCountdown(sf::Time::Zero),
      mMissileAmmo(2),
      mFireRateLevel(1),
      mIsFiring(false),
      mIsLaunchingMissile(false),
      mSpreadLevel(1),
      mIsMarkedForRemoval(false)
{
    util::centerOrigin(mSprite);

    // Texts setup

    std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
    mHealthDisplay = healthDisplay.get();
    attachChild(healthDisplay.release());

    std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
    mMissileDisplay = missileDisplay.get();
    attachChild(missileDisplay.release());


    // Commands setup

    mFireCommand.category = Category::SceneAirLayer;
    mFireCommand.action = 
    [this, &textures] (SceneNode& node, const sf::Time) 
    {
        createBullet(node, textures);
    };

    mMissileCommand.category = Category::SceneAirLayer;
    mMissileCommand.action = 
    [this, &textures] (SceneNode& node, const sf::Time)
    {
        createProjectile(node, textures, Projectile::Missile, 0.f, 0.5f);  
    };

    mDropPickupCommand.category = Category::SceneAirLayer;
    mDropPickupCommand.action = [this, &textures] (SceneNode& node, const sf::Time)
    {
        createPickup(node, textures);
    };
}


unsigned int Aircraft::getCategory() const
{
    return (mType == Aircraft::Eagle) 
            ? Category::PlayerAircraft 
            : Category::EnemyAircraft;
}


float Aircraft::getMaxSpeed() const
{
    return DATA_TABLE[mType].speed;
}


sf::FloatRect Aircraft::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}


void Aircraft::fire()
{
    if (DATA_TABLE[mType].fireInterval != sf::Time::Zero)
    {
        mIsFiring = true;
    }
}


void Aircraft::launchMissile()
{
    if (mMissileAmmo > 0)
    {
        mIsLaunchingMissile = true;
        --mMissileAmmo;
    }
}


void Aircraft::collectMissiles(const int count)
{
    mMissileAmmo += count;
    if (mMissileAmmo > 5)
    {
        mMissileAmmo = 5;
    }
}


void Aircraft::increaseSpread()
{
    if (mSpreadLevel < 3)
    {
        ++mSpreadLevel;
    }
}


void Aircraft::increaseFireRate()
{
    if (mFireRateLevel < 10)
    {
        ++mFireRateLevel;
    }
}


void Aircraft::updateCurrent(const sf::Time dt, CommandQueue& commands)
{
    if (isDestroyed())
    {
        checkPickupDrop(commands);
        mIsMarkedForRemoval = true;
        return;
    }
    checkProjectileLaunch(dt, commands);
    updateTexts();
    updateMovementPattern(dt);
    Entity::updateCurrent(dt, commands);
}


void Aircraft::updateMovementPattern(const sf::Time dt)
{
    // Movement pattern for the enemy's aircraft

    const std::vector<Direction>& directions = DATA_TABLE[mType].directions;
    if (!directions.empty())
    {
        if (mTravelledDistance > directions[mDirectionIndex].distance)
        {
            mDirectionIndex = (mDirectionIndex + 1) % directions.size();
            mTravelledDistance = 0.f;
        }

        // Compute velocity from direction
        float radians = util::toRadian(directions[mDirectionIndex].angle + 90.f);
        float velocityX = getMaxSpeed() * std::cos(radians);
        float velocityY = getMaxSpeed() * std::sin(radians);

        setVelocity(velocityX, velocityY);

        mTravelledDistance += getMaxSpeed() * dt.asSeconds();
    }
}


void Aircraft::updateTexts()
{
    mHealthDisplay->setString(util::toString(getHitpoints()) + " HP");
    mHealthDisplay->setPosition(0.f, 50.f);
    mHealthDisplay->setRotation(-getRotation());

    if (isAllied())
    {
        mMissileDisplay->setString(util::toString(mMissileAmmo)+ " M");
        mMissileDisplay->setPosition(0.f, 72.f);
        mMissileDisplay->setRotation(-getRotation());
    }
}


void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}


void Aircraft::checkProjectileLaunch(const sf::Time dt, CommandQueue& commands)
{
    // Enemies try to fire all the time
    if (!isAllied())
    {
        fire();
    }

    // Check automatic gunfire
    if (mIsFiring && mFireCountdown <= sf::Time::Zero)
    {
        commands.push(mFireCommand);
        mFireCountdown += sf::seconds(1.f / (mFireRateLevel + 1));
        mIsFiring = false;
    }
    else if (mFireCountdown > sf::Time::Zero)
    {
        mFireCountdown -= dt;
        mIsFiring = false;
    }

    // Check for missile launch
    if (mIsLaunchingMissile)
    {
        commands.push(mMissileCommand);
        mIsLaunchingMissile = false;
    }
}


void Aircraft::checkPickupDrop(CommandQueue& commands)
{
    if (!isAllied() && rand() % 2 == 0)
    {
        commands.push(mDropPickupCommand);
    }
}


void Aircraft::createBullet(SceneNode& node, const TextureHolder& textures)
{
    Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

    switch (mSpreadLevel)
    {
        case 1:
            createProjectile(node, textures, type,     0.f,  0.5f);
            break;

        case 2:
            createProjectile(node, textures, type,  -0.33f, 0.33f);
            createProjectile(node, textures, type,   0.33f, 0.33f);
            break;

        case 3:
            createProjectile(node, textures, type,  -0.5f, 0.33f);
            createProjectile(node, textures, type,    0.f,  0.5f);
            createProjectile(node, textures, type,   0.5f, 0.33f);
            break;

        default:
            assert(false);
    }
}


void Aircraft::createProjectile(SceneNode& node, 
                                const TextureHolder& textures,
                                const Projectile::Type type,
                                const float offsetX,
                                const float offsetY)
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(offsetX * mSprite.getGlobalBounds().width, 
                        offsetY * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(projectile.release());
}


void Aircraft::createPickup(SceneNode& node, const TextureHolder& textures)
{
    Pickup::Type type = (Pickup::Type)(rand() % Pickup::TypeCount);

    std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
    pickup->setPosition(getWorldPosition());
    pickup->setVelocity(0.f, 1.f);
    node.attachChild(pickup.release());
}


bool Aircraft::isAllied() const
{
    return mType == Eagle;
}


bool Aircraft::isMarkedForRemoval() const
{
    return mIsMarkedForRemoval;
}
