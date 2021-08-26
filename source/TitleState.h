#pragma once
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "State.h"


class TitleState: public State
{
public:
                    TitleState(StateStack& stack, Context context);

    virtual bool    handleEvent(const sf::Event& event);
    virtual bool    update(const sf::Time dt);
    virtual void    draw();


private:
    sf::Sprite      mBackgroundSprite;
    sf::Text        mText;
                    
    bool            mShowText;
    sf::Time        mTextEffectTime;
};

