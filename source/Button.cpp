#include "Button.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "SFML/Graphics/RenderTarget.hpp"


namespace gui
{


Button::Button(const FontHolder& fonts, const TextureHolder& textures)
    : mCallback(nullptr),
      mButtonsTexture(textures.get(Textures::Buttons)),
      mText("", fonts.get(Fonts::Main), 16),
      mIsToggle(false)
{
    mSprite.setTexture(mButtonsTexture);
    changeTexture(Normal);

    sf::FloatRect bounds(mSprite.getLocalBounds());
    mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}


void Button::handleEvent(const sf::Event&)
{
    // Do nothing
}


void Button::setCallback(Callback callback)
{
    mCallback = callback;
}


void Button::setText(const std::string& text)
{
    mText.setString(text);
    util::centerOrigin(mText);
}


void Button::setToggle(bool flag)
{
    mIsToggle = flag;
}


bool Button::isSelectable() const
{
    return true;
}


void Button::select()
{
    Component::select();

    changeTexture(Selected);
}


void Button::deselect()
{
    Component::deselect();

    changeTexture(Normal);
}


void Button::activate()
{
    Component::activate();

    if (mCallback)
    {
        mCallback();
    }

    mIsToggle ? changeTexture(Pressed) : deactivate();

}


void Button::deactivate()
{
    Component::deactivate();

    if (mIsToggle)
    {
        isSelected() 
        ? changeTexture(Selected) 
        : changeTexture(Normal);
    }
}


void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(mSprite, states);
    target.draw(mText, states);
}


void Button::changeTexture(const Type buttonType)
{
    mSprite.setTextureRect(sf::IntRect(0, 50*buttonType, 200, 50));
}


} // namespace gui

