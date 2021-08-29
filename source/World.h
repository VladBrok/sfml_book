#pragma once
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Aircraft.h"
#include "CommandQueue.h"
#include "BloomEffect.h"

#include "SFML/System/NonCopyable.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <array>

class SoundPlayer;


class World
{
public:
    explicit                           World(sf::RenderTarget& outputTarget, 
                                             FontHolder& fonts,
                                             SoundPlayer& sounds);

    void                               update(const sf::Time dt);
    void                               draw();

    CommandQueue&                      getCommandQueue();


private:
    void                               loadTextures();
    void                               buildScene();
    void                               handleCollisions();

    void                               adaptPlayerVelocity();
    void                               adaptPlayerPosition();

    void                               spawnEnemies();
    void                               addEnemySpawnPoint(const Aircraft::Type type,
                                                          const float relatedPosX,
                                                          const float relatedPosY);
    void                               addEnemies();
    void                               removeEnemiesOutsideView();
    void                               guideMissiles();

    sf::FloatRect                      getViewBounds() const;
    sf::FloatRect                      getBattlefieldBounds() const;

    void                               updateSounds();

private:
    enum Layer
    {
        Background,
        LowerAir,
        UpperAir,

        LayerCount
    };


    struct SpawnPoint
    {
        SpawnPoint(const Aircraft::Type type,
                   const float x,
                   const float y)
            : type(type),
              x(x),
              y(y)
        {
        }

        Aircraft::Type type;
        float          x;
        float          y;
    };


private:
    sf::RenderTarget&                  mTarget;
    sf::RenderTexture                  mSceneTexture;
    sf::View                           mWorldView;
    TextureHolder                      mTextures;
    FontHolder&                        mFonts;

    SceneNode                          mSceneGraph;
    std::array<SceneNode*, LayerCount> mSceneLayers;
    CommandQueue                       mCommandQueue;

    sf::FloatRect                      mWorldBounds;
    sf::Vector2f                       mSpawnPosition;
    float                              mScrollSpeed;
    Aircraft*                          mPlayerAircraft; // FIXME: This pointer becomes invalid as soon as the player dies

    std::list<SpawnPoint>              mEnemySpawnPoints;
    std::list<Aircraft*>               mActiveEnemies;

    BloomEffect                        mBloomEffect;
    SoundPlayer&                       mSounds;
};



bool matchesCategories(SceneNode::NodePair& colliders, 
                       Category::Type type1, 
                       Category::Type type2);