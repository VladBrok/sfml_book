#pragma once
#include "SceneNode.h"
#include "ResourceIdentifiers.h"
#include "SFML/Graphics/Text.hpp"


class TextNode: public SceneNode
{
public:
    explicit        TextNode(const FontHolder& fonts, const std::string& text);
    void            setString(const std::string& string);


private:
    virtual void    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
    sf::Text        mText;
};