#include "ParticleNode.h"
#include "DataTables.h"
#include "ResourceHolder.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"


namespace
{
    const std::vector<ParticleData> DATA_TABLE(initParticleData());
}


ParticleNode::ParticleNode(const Particle::Type type, const TextureHolder& textures)
    : mTexture(textures.get(Textures::Particle)),
      mType(type),
      mVertexArray(sf::Quads),
      mNeedsVertexUpdate(true)
{
}
   

void ParticleNode::addParticle(const sf::Vector2f& position)
{
    Particle particle;
    particle.position = position;
    particle.color = DATA_TABLE[mType].color;
    particle.lifetime = DATA_TABLE[mType].lifetime;

    mParticles.push_back(particle);
}


Particle::Type ParticleNode::getParticleType() const
{
    return mType;
}


unsigned int ParticleNode::getCategory() const
{
    return Category::ParticleSystem;
}


void ParticleNode::updateCurrent(const sf::Time dt, CommandQueue&)
{
    // Remove expired particles
    while (!mParticles.empty() && mParticles.front().lifetime <= sf::Time::Zero)
    {
        mParticles.pop_front();
    }

    // Decrease lifetime of existing particles
    for (auto& particle: mParticles)
    {
        particle.lifetime -= dt;
    }
    mNeedsVertexUpdate = true;
}


void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mNeedsVertexUpdate)
    {
        computeVertices();
        mNeedsVertexUpdate = false;
    }

    states.texture = &mTexture;
    target.draw(mVertexArray, states);
}


void ParticleNode::addVertex(const float worldX, 
                             const float worldY, 
                             const float texCoordX, 
                             const float texCoordY, 
                             const sf::Color& color) const
{
    sf::Vertex vertex;
    vertex.color = color;
    vertex.position = sf::Vector2f(worldX, worldY);
    vertex.texCoords = sf::Vector2f(texCoordX, texCoordY);

    mVertexArray.append(vertex);
}


void ParticleNode::computeVertices() const
{
    sf::Vector2f size(mTexture.getSize());
    sf::Vector2f half = size / 2.f;

    // Refill vertex array
    mVertexArray.clear();
    for (const Particle& particle: mParticles)
    {
        sf::Vector2f pos(particle.position);
        sf::Color color(particle.color);

        float ratio = particle.lifetime.asSeconds() / DATA_TABLE[mType].lifetime.asSeconds();
        color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));
    
        addVertex(pos.x - half.x, pos.y - half.y, 0.f,    0.f,    color);
        addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f,    color);
        addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
        addVertex(pos.x - half.x, pos.y + half.y, 0.f,    size.y, color);
    }
}