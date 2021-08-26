#include "SceneNode.h"
#include "Command.h"
#include "Utility.h"

#include <algorithm>
#include <cassert>
#include <iostream>


SceneNode::SceneNode(const Category::Type defaultCategory)
    : mParent(nullptr),
      mDefaultCategory(defaultCategory)
{
}


void SceneNode::attachChild(SceneNode* node)
{
    assert(node != nullptr);

    NodePtr newNode(node);
    newNode->mParent = this;
    mChildren.push_back(std::move(newNode));
}


SceneNode::NodePtr SceneNode::detachChild(const SceneNode& node)
{
    auto found = std::find_if(mChildren.begin(),
                              mChildren.end(), 
                              [&](NodePtr& child) { return child.get() == &node; });
    assert(found != mChildren.end());

    NodePtr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);

    return result;
}


void SceneNode::update(const sf::Time dt, CommandQueue& commands)
{
    updateCurrent(dt, commands);
    updateChildren(dt, commands);
}


void SceneNode::updateCurrent(const sf::Time, CommandQueue&)
{
    // Do nothing by default
}


void SceneNode::updateChildren(const sf::Time dt, CommandQueue& commands)
{
    for (const NodePtr& child: mChildren)
    {
        child->update(dt, commands);
    }
}


void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    drawCurrent(target, states);
    drawChildren(target, states);
}


void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
    // Do nothing by default
}


void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const NodePtr& child: mChildren)
    {
        child->draw(target, states);
    }
}


const sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}


const sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    for (const SceneNode* node = this; node != nullptr; node = node->mParent)
    {
        transform = node->getTransform() * transform;
    }

    return transform;
}


unsigned int SceneNode::getCategory() const
{
    return mDefaultCategory;
}


sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}


void SceneNode::onCommand(const Command& command, const sf::Time dt)
{
    // Command current node, if category matches (comparing categories using bitwise operator '&')
    if (command.category & getCategory())
    {
        command.action(*this, dt);
    }

    // Command children
    for (const NodePtr& child: mChildren)
    {
        child->onCommand(command, dt);
    }
}


void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<NodePair>& collisionPairs)
{
    checkNodeCollision(sceneGraph, collisionPairs);

    for (auto& child: sceneGraph.mChildren)
    {
        checkSceneCollision(*child, collisionPairs);
    }
}


void SceneNode::removeWrecks()
{
    auto wrecksBegin = std::remove_if(mChildren.begin(), 
                                      mChildren.end(),
                                      std::mem_fn(&SceneNode::isMarkedForRemoval));
    mChildren.erase(wrecksBegin, mChildren.end());

    for (auto& child: mChildren)
    {
        child->removeWrecks();
    }
}


void SceneNode::checkNodeCollision(SceneNode& node, std::set<NodePair>& collisionPairs)
{
    if (this != &node && collision(*this, node) && !node.isDestroyed() && !isDestroyed())
    {
        /*
            'minmax' and container 'set' are used to ensure that collision between nodes A and B
            is inserted only ones (and not twice as A-B and B-A pairs)
        */
        collisionPairs.insert(std::minmax(this, &node));
    }

    for (auto& child: mChildren)
    {
        child->checkNodeCollision(node, collisionPairs);
    }
}


bool SceneNode::isDestroyed() const
{
    return false;
}


bool SceneNode::isMarkedForRemoval() const
{
    return isDestroyed();
}


float distance(const SceneNode& lhs, const SceneNode& rhs)
{
    return util::getVectorLength(rhs.getWorldPosition() - lhs.getWorldPosition());
}


bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
   // std::cout << typeid(lhs).name() << ' ' << typeid(rhs).name() << '\n';
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}
