#include "TitleState.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "SFML/Graphics/RenderWindow.hpp"


TitleState::TitleState(StateStack& stack, Context context)
    : State(stack, context),
      mShowText(true),
      mTextEffectTime(sf::Time::Zero)
{
    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    mText.setFont(context.fonts->get(Fonts::Main));
    mText.setString("Press any key to start");
    mText.setCharacterSize(45);
    util::centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize() / 2.f);
}


bool TitleState::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyReleased)
    {
        requestStackPop();
        requestStackPush(States::Menu);
    }

    return true;
}


bool TitleState::update(const sf::Time dt)
{
    mTextEffectTime += dt;

    if (mTextEffectTime >= sf::seconds(0.5f))
    {
        mShowText = !mShowText;
        mTextEffectTime = sf::Time::Zero;
    }

    return true;
}


void TitleState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);

    if (mShowText)
    {
        window.draw(mText);
    }
}