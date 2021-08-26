#include "Label.h"
#include "SFML/Graphics/RenderTarget.hpp"


namespace gui
{


Label::Label(const std::string& text, const FontHolder& fonts)
    : mText(text, fonts.get(Fonts::Main), 16)
{
}


void Label::handleEvent(const sf::Event&)
{
    // Do nothing
}


bool Label::isSelectable() const
{
    return false;
}


void Label::setText(const std::string& text)
{
    mText.setString(text);
}


void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mText, states);
}


} // namespace gui
