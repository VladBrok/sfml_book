#pragma once
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/Time.hpp"
#include "Category.h"

#include <memory>
#include <vector>
#include <set>

struct Command;
class CommandQueue;


class SceneNode: public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode> NodePtr;
    typedef std::pair<SceneNode*, SceneNode*> NodePair;


public:
                            SceneNode(const Category::Type defaultCategory = Category::None);
    virtual                 ~SceneNode() {};
    
    void                    attachChild(SceneNode* node);
    NodePtr                 detachChild(const SceneNode& node);
                            
    void                    update(const sf::Time dt, CommandQueue& commands);
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;
                            
    const sf::Vector2f      getWorldPosition() const;
    const sf::Transform     getWorldTransform() const;
    virtual unsigned int    getCategory() const;
    virtual sf::FloatRect   getBoundingRect() const;

    void                    onCommand(const Command& command, const sf::Time dt);
    void                    checkSceneCollision(SceneNode& sceneGraph, 
                                                std::set<NodePair>& collisionPairs);
    void                    removeWrecks();

    virtual bool            isDestroyed() const;
    virtual bool            isMarkedForRemoval() const;
                            
                            
private:                    
    virtual void            updateCurrent(const sf::Time dt, CommandQueue& commands);
    void                    updateChildren(const sf::Time dt, CommandQueue& commands);
                            
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void                    drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

    void                    checkNodeCollision(SceneNode& node, 
                                               std::set<NodePair>& collisionPairs);


private:
    std::vector<NodePtr>    mChildren;
    SceneNode*              mParent;
    Category::Type          mDefaultCategory;
};



float distance(const SceneNode& lhs, const SceneNode& rhs);
bool collision(const SceneNode& lhs, const SceneNode& rhs);
