#pragma once
#include "Component.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"
#include "SFML/Graphics/Text.hpp"


namespace gui
{


class Label: public Component
{
public:
    typedef std::shared_ptr<Label> LabelPtr;


                    Label(const std::string& text, const FontHolder& fonts);

    virtual void    handleEvent(const sf::Event& event);
    virtual bool    isSelectable() const;
    void            setText(const std::string& text);


private:
    virtual void    draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
    sf::Text        mText;
};


} // namespace gui

