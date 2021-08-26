#include "Projectile.h"
#include "DataTables.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "EmitterNode.h"
#include "SFML/Graphics/RenderTarget.hpp"


namespace
{
    const std::vector<ProjectileData> DATA_TABLE(initProjectileData());
}


Projectile::Projectile(const Type type, const TextureHolder& textures)
    : Entity(1),
      mType(type),
      mSprite(textures.get(DATA_TABLE[type].textureId), DATA_TABLE[type].textureRect)
{
    util::centerOrigin(mSprite);

    // Add particle system for missiles
    if (isGuided())
    {
        std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke));
        smoke->setPosition(0.f, getBoundingRect().height / 2.f);
        attachChild(smoke.release());

        std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Propellant));
        propellant->setPosition(0.f, getBoundingRect().height / 2.f);
        attachChild(propellant.release());
    }
}


void Projectile::guideTowards(const sf::Vector2f& position)
{
    assert(isGuided());
    mTargetDirection = util::getNormalizedVector(position - getWorldPosition());
}


bool Projectile::isGuided() const
{
    return mType == Missile;
}
     

unsigned int Projectile::getCategory() const
{
    return (mType == EnemyBullet) ? Category::EnemyProjectile : Category::AlliedProjectile;
}


sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}


float Projectile::getMaxSpeed() const
{
    return DATA_TABLE[mType].speed;
}


int	 Projectile::getDamage() const
{
    return DATA_TABLE[mType].damage;
}


void Projectile::updateCurrent(const sf::Time dt, CommandQueue& commands)
{
	if (isGuided())
	{
		const float APPROACH_RATE = 200.f;

        sf::Vector2f newVelocity(
            util::getNormalizedVector(APPROACH_RATE
                                      * dt.asSeconds()      
                                      * mTargetDirection      
                                      + getVelocity())); 
        newVelocity *= getMaxSpeed();
        float angle = std::atan2f(newVelocity.y, newVelocity.x);

        setRotation(util::toDegree(angle) + 90.f);
        setVelocity(newVelocity);                               
	}

	Entity::updateCurrent(dt, commands);
}


void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
