#pragma once
#include <functional>
#include <cassert>
#include "SFML/System/Time.hpp"

class SceneNode;


struct Command
{
                                                    Command();

    std::function<void(SceneNode&, const sf::Time)> action;
    unsigned int                                    category;
};


template <typename SceneNodeDerived>

std::function<void(SceneNode&, const sf::Time)> 
derivedAction(std::function<void(SceneNodeDerived&, const sf::Time)> function)
{
    return [=] (SceneNode& node, const sf::Time dt)
    {
        assert(dynamic_cast<SceneNodeDerived*>(&node) != nullptr);

        function(static_cast<SceneNodeDerived&>(node), dt);
    };
}