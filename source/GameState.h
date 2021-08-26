#pragma once
#include "State.h"
#include "World.h"
#include "Player.h"


class GameState: public State
{
public:
               GameState(StateStack& stack, Context context);

    bool       handleEvent(const sf::Event& event);
    bool       update(const sf::Time dt);
    void       draw();


private:
    World      mWorld;
    Player*    mPlayer;
};

