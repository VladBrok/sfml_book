#pragma once
#include "State.h"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "Container.h"


class MenuState: public State
{
public:
                             MenuState(StateStack& stack, Context context);
                             
    virtual bool             handleEvent(const sf::Event& event);
    virtual bool             update(const sf::Time dt);
    virtual void             draw();


private:
    sf::Sprite               mBackgroundSprite;
    gui::Container           mGuiContainer;
};

