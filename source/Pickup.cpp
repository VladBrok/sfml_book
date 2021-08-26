#include "Pickup.h"
#include "ResourceHolder.h"
#include "DataTables.h"
#include "Utility.h"
#include "SFML/Graphics/RenderWindow.hpp"


namespace
{
    const std::vector<PickupData> DATA_TABLE(initPickupData());
}


Pickup::Pickup(const Type type, const TextureHolder& textures)
    : Entity(1),
      mType(type),
      mSprite(textures.get(DATA_TABLE[type].textureId), DATA_TABLE[type].textureRect)
{
    util::centerOrigin(mSprite);
}


unsigned int Pickup::getCategory() const
{
    return Category::Pickup;
}


sf::FloatRect Pickup::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}


void Pickup::apply(Aircraft& player) const
{
    DATA_TABLE[mType].action(player);
}


void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}