#pragma once
#include "State.h"
#include "Container.h"
#include "SFML/Graphics/Text.hpp"


class PauseState: public State
{
public:
                      PauseState(StateStack& stack, Context context);
                      ~PauseState();
                      
    virtual bool      handleEvent(const sf::Event& event);
    virtual bool      update(const sf::Time dt);
    virtual void      draw();
                      
                      
private:              
    sf::Text          mPauseText;
    gui::Container    mGuiContainer;
};

