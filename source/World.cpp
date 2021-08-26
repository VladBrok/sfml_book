#include "World.h"
#include "Utility.h"
#include "Pickup.h"
#include "ParticleNode.h"
#include "SFML/Graphics/RenderWindow.hpp"

#include <iostream>


World::World(sf::RenderWindow& window, FontHolder& fonts)
    : mWindow(window),
      mWorldView(window.getDefaultView()),
      mFonts(fonts),
      mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 5000.f),
      mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f),
      mScrollSpeed(-50.f),
      mPlayerAircraft(nullptr)
{
    srand((unsigned)time(nullptr));

    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}


void World::update(const sf::Time dt)
{
    mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
    mPlayerAircraft->setVelocity(0.f, 0.f);
    
    // Transfer commands to scene graph
    while (!mCommandQueue.isEmpty())
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }
    adaptPlayerVelocity();

    handleCollisions();

    mSceneGraph.removeWrecks();
    removeEnemiesOutsideView();

    spawnEnemies();
    guideMissiles();

    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();
}


void World::adaptPlayerVelocity()
{
    sf::Vector2f velocity(mPlayerAircraft->getVelocity());

    // If moving diagonally, reduce velocity
    if (velocity.x != 0 && velocity.y != 0)
    {
        mPlayerAircraft->setVelocity(velocity / std::sqrtf(2.f));
    }

    if (velocity.y <= 0.f)
    {
        mPlayerAircraft->accelerate(0.f, mScrollSpeed);
    }
}


void World::adaptPlayerPosition()
{
    // Keep player's position inside the screen bounds, at least BORDER_DISTANCE units from the border

    const float BORDER_DISTANCE = 40.f;
    sf::Vector2f position(mPlayerAircraft->getPosition());
    sf::FloatRect bounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());

    position.x = std::max(position.x, bounds.left + BORDER_DISTANCE);
    position.x = std::min(position.x, bounds.left + bounds.width - BORDER_DISTANCE);
    position.y = std::max(position.y, bounds.top + BORDER_DISTANCE);
    position.y = std::min(position.y, bounds.top + bounds.height - BORDER_DISTANCE);

    mPlayerAircraft->setPosition(position);
}


void World::spawnEnemies()
{
    while (!mEnemySpawnPoints.empty()
               && mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
    {
        SpawnPoint spawn(mEnemySpawnPoints.back());

        std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type, mTextures, mFonts));
        enemy->setPosition(spawn.x, spawn.y);
        enemy->setRotation(180.f);
        
        mSceneLayers[UpperAir]->attachChild(enemy.release());

        mEnemySpawnPoints.pop_back();
    }
}


void World::addEnemySpawnPoint(const Aircraft::Type type,
                               const float relatedPosX,
                               const float relatedPosY)
{
    SpawnPoint spawn(type, mSpawnPosition.x + relatedPosX, mSpawnPosition.y - relatedPosY);

    mEnemySpawnPoints.push_back(spawn);
}


void World::addEnemies()
{
    // Add enemies to the spawn point container
    addEnemySpawnPoint(Aircraft::Raptor,    0.f,  500.f);
    addEnemySpawnPoint(Aircraft::Raptor,    0.f, 1000.f);
    addEnemySpawnPoint(Aircraft::Raptor, +100.f, 1150.f);
    addEnemySpawnPoint(Aircraft::Raptor, -100.f, 1150.f);
    addEnemySpawnPoint(Aircraft::Avenger,  70.f, 1500.f);
    addEnemySpawnPoint(Aircraft::Avenger, -70.f, 1500.f);
    addEnemySpawnPoint(Aircraft::Avenger, -70.f, 1710.f);
    addEnemySpawnPoint(Aircraft::Avenger,  70.f, 1700.f);
    addEnemySpawnPoint(Aircraft::Avenger,  30.f, 1850.f);
    addEnemySpawnPoint(Aircraft::Raptor,  300.f, 2200.f);
    addEnemySpawnPoint(Aircraft::Raptor, -300.f, 2200.f);
    addEnemySpawnPoint(Aircraft::Raptor,    0.f, 2200.f);
    addEnemySpawnPoint(Aircraft::Raptor,    0.f, 2500.f);
    addEnemySpawnPoint(Aircraft::Avenger,-300.f, 2700.f);
    addEnemySpawnPoint(Aircraft::Avenger,-300.f, 2700.f);
    addEnemySpawnPoint(Aircraft::Raptor,    0.f, 3000.f);
    addEnemySpawnPoint(Aircraft::Raptor,  250.f, 3250.f);
    addEnemySpawnPoint(Aircraft::Raptor, -250.f, 3250.f);
    addEnemySpawnPoint(Aircraft::Avenger,   0.f, 3500.f);
    addEnemySpawnPoint(Aircraft::Avenger,   0.f, 3700.f);
    addEnemySpawnPoint(Aircraft::Raptor,    0.f, 3800.f);
    addEnemySpawnPoint(Aircraft::Avenger,   0.f, 4000.f);
    addEnemySpawnPoint(Aircraft::Avenger,-200.f, 4200.f);
    addEnemySpawnPoint(Aircraft::Raptor,  200.f, 4200.f);
    addEnemySpawnPoint(Aircraft::Raptor,    0.f, 4400.f);

    /*
        Sort all enemies according to their Y value, 
        such that enemies at the back are checked first for spawning
    */
    mEnemySpawnPoints.sort([] (SpawnPoint& lhs, SpawnPoint& rhs) 
    { 
        return lhs.y < rhs.y;
    });
}


void World::removeEnemiesOutsideView()
{
    Command command;
    command.category = Category::EnemyAircraft | Category::Projectile;
    command.action = derivedAction<Entity>(
    [this] (Entity& entity, sf::Time)
    {
        if (!getBattlefieldBounds().intersects(entity.getBoundingRect()))
        {
            entity.destroy();
        }
    });
    mCommandQueue.push(command);
}


void World::guideMissiles()
{
    // Collecting the active enemies
    Command enemyCollector;
    enemyCollector.category = Category::EnemyAircraft;
    enemyCollector.action = derivedAction<Aircraft>(
    [this] (Aircraft& enemy, const sf::Time)
    {
        if (!enemy.isDestroyed())
        {
            mActiveEnemies.push_back(&enemy); // FIXME !!!!!: pushing in the local list captured by reference
        }
    });

    // Finding the closest enemy to guide missiles towards him
    Command missileGuider;
    missileGuider.category = Category::AlliedProjectile;
    missileGuider.action = derivedAction<Projectile>(
    [this] (Projectile& missile, const sf::Time)
    {
        // Ignore unguided bullets
        if (!missile.isGuided())
        {
            return;
        }

        float minDistance = std::numeric_limits<float>::max();
        Aircraft* closestEnemy = nullptr;

        for (Aircraft* enemy: mActiveEnemies)
        { 
            float distanceToEnemy = distance(missile, *enemy);
            if (distanceToEnemy < minDistance)
            {
                closestEnemy = enemy;
                minDistance = distanceToEnemy;
            }
        }
        if (closestEnemy)
        {
            missile.guideTowards(closestEnemy->getWorldPosition());
        }
    });

    mCommandQueue.push(enemyCollector);
    mCommandQueue.push(missileGuider);
    mActiveEnemies.clear();
}


sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f,
                         mWorldView.getSize());
}


sf::FloatRect World::getBattlefieldBounds() const
{
    // Return view bounds + some area at the top where enemies can spawn

    float additionalArea = 100.f;
    sf::FloatRect bounds(getViewBounds());
    bounds.top -= additionalArea;
    bounds.height += additionalArea;

    return bounds;
}


void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
    mWindow.setView(mWindow.getDefaultView());
}


CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}


void World::loadTextures()
{
    mTextures.load(Textures::Entities,   "Media/Textures/Entities.png");
    mTextures.load(Textures::Jungle,     "Media/Textures/Jungle.png");
    mTextures.load(Textures::FinishLine, "Media/Textures/FinishLine.png");
    mTextures.load(Textures::Particle,   "Media/Textures/Particle.png");
}


void World::buildScene()
{
    // Initialize the different layers
    for (std::size_t i = 0; i < LayerCount; ++i)
    {
        Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

        SceneNode::NodePtr node(new SceneNode(category));
        mSceneLayers[i] = node.get();
        mSceneGraph.attachChild(node.release());
    }


    // Prepare the tiled background
    sf::Texture& jungleTexture = mTextures.get(Textures::Jungle);
    jungleTexture.setRepeated(true);

    float viewHeight = mWorldView.getSize().y;
    sf::IntRect textureRect(mWorldBounds);
    textureRect.height += static_cast<int>(viewHeight);

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> jungleSprite(new SpriteNode(jungleTexture, textureRect));
    jungleSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
    mSceneLayers[Background]->attachChild(jungleSprite.release());


    // Add the finish line to the scene
    sf::Texture& finishTexture = mTextures.get(Textures::FinishLine);
    std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishTexture));
    finishSprite->setPosition(0.f, -76.f);
    mSceneLayers[Background]->attachChild(finishSprite.release());


    // Add particle systems
    std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Smoke, mTextures));
    mSceneLayers[LowerAir]->attachChild(smokeNode.release());

    std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Propellant, mTextures));
    mSceneLayers[LowerAir]->attachChild(propellantNode.release());


    // Add player's aircraft
    std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures, mFonts));
    mPlayerAircraft = leader.get();
    mPlayerAircraft->setPosition(mSpawnPosition);
    mSceneLayers[UpperAir]->attachChild(leader.release());


    // Add enemy aircraft
    addEnemies();
}


void World::handleCollisions()
{
    std::set<SceneNode::NodePair> collisionPairs;
    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);


    for (SceneNode::NodePair pair: collisionPairs)
    {
        if (matchesCategories(pair, Category::PlayerAircraft, Category::EnemyAircraft))
        {
            Aircraft* player = dynamic_cast<Aircraft*>(pair.first);
            Aircraft* enemy = dynamic_cast<Aircraft*>(pair.second);
            assert(player);
            assert(enemy);

            // Player damage = enemy's remaining HP
            player->damage(enemy->getHitpoints());
            enemy->destroy();
        }
        else if (matchesCategories(pair, Category::PlayerAircraft, Category::Pickup))
        {
            Aircraft* player = dynamic_cast<Aircraft*>(pair.first);
            Pickup* pickup = dynamic_cast<Pickup*>(pair.second);
            assert(player);
            assert(pickup);

            // Apply pickup effect to player, destroy pickup
            pickup->apply(*player);
            pickup->destroy();
        }
        else if (matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile)
                    || matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile))
        {
            Aircraft* aircraft = dynamic_cast<Aircraft*>(pair.first);
            Projectile* projectile = dynamic_cast<Projectile*>(pair.second);
            assert(aircraft);
            assert(projectile);

            // Apply projectile damage to aircraft, destroy projectile
            aircraft->damage(projectile->getDamage());
            projectile->destroy();
        }                                    
    }
}


bool matchesCategories(SceneNode::NodePair& colliders, 
                       Category::Type type1, 
                       Category::Type type2)
{
    unsigned category1 = colliders.first->getCategory();
    unsigned category2 = colliders.second->getCategory();

    if ((category1 & type1) && (category2 & type2))
    {
        return true;
    }
    else if ((type1 & category2) && (type2 & category1))
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    else
    {
        return false;
    }
}