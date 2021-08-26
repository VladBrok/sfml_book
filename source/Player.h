#pragma once

#include "SFML/Window/Event.hpp"
#include <unordered_map>
#include "Command.h"

class CommandQueue;


class Player
{
public:
    enum Action
    {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        Fire,
        LaunchMissile,

        ActionCount
    };


public:
                                                  Player();

    void                                          handleEvent(const sf::Event& event, 
                                                              CommandQueue& commandQueue);
    void                                          handleRealtimeInput(CommandQueue& commandQueue);
    void                                          assignKey(const Action action, 
                                                            const sf::Keyboard::Key key);
    sf::Keyboard::Key                             getAssignedKey(const Action action) const;


private:
    void                                          initKeyBindings();
    void                                          initActionBindings();

    bool                                          isRealtimeAction(const Action action);


private:
    std::unordered_map<sf::Keyboard::Key, Action> mKeyBindings;
    std::unordered_map<Action, Command>           mActionBindings;
};

