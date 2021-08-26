#pragma once
#include <memory>
#include "ResourceIdentifiers.h"
#include "StateIdentifiers.h"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"

namespace sf
{
    class RenderWindow;
}
class Player;
class StateStack;


class State
{
public:
    typedef std::unique_ptr<State> StatePtr;

    struct Context
    {
                          Context(sf::RenderWindow& window,
                                  TextureHolder& textures,
                                  FontHolder& fonts,
                                  Player& player);
        sf::RenderWindow* window;
        TextureHolder*    textures;
        FontHolder*       fonts;
        Player*           player;
    };


public:
                      State(StateStack& stack, Context context);
    virtual bool      handleEvent(const sf::Event& event) = 0;
    virtual bool      update(const sf::Time dt) = 0;
    virtual void      draw() = 0;
                      
                      
protected:            
    void              requestStackPush(States::ID stateId);
    void              requestStackPop();
    void              requestStackClear();
                      
    const Context&    getContext() const;
                      
private:              
    StateStack*       mStack;
    Context           mContext;
};