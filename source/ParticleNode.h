#pragma once
#include "SceneNode.h"
#include "ResourceIdentifiers.h"
#include "Particle.h"

#include "SFML/Graphics/VertexArray.hpp"

#include <deque>


class ParticleNode: public SceneNode
{
public:
                               ParticleNode(const Particle::Type type, 
                                            const TextureHolder& textures);
                               
    void                       addParticle(const sf::Vector2f& position);
    Particle::Type             getParticleType() const;
    virtual unsigned int       getCategory() const;
                               
                               
private:                       
    virtual void               updateCurrent(const sf::Time dt, CommandQueue& commands);
    virtual void               drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
                               
    void                       addVertex(const float worldX, 
                                         const float worldY, 
                                         const float texCoordX, 
                                         const float texCoordY, 
                                         const sf::Color& color) const;
    void                       computeVertices() const;


private:
    std::deque<Particle>       mParticles;
    const sf::Texture&         mTexture;
    Particle::Type             mType;

    mutable sf::VertexArray    mVertexArray;
    mutable bool               mNeedsVertexUpdate;
};

