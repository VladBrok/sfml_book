#pragma once
#include "Component.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <functional>


namespace gui
{


class Button: public Component
{
public:
    enum Type
    {
        Normal,
        Selected,
        Pressed,

        ButtonCount
    };


public:
    typedef std::shared_ptr<Button> ButtonPtr;
    typedef std::function<void()> Callback;


                          Button(const FontHolder& fonts, const TextureHolder& textures);

    virtual void          handleEvent(const sf::Event& event);

    void                  setCallback(Callback callback);
    void                  setText(const std::string& text);
    void                  setToggle(bool flag);

    virtual bool          isSelectable() const;
    virtual void          select();
    virtual void          deselect();
                          
    virtual void          activate();
    virtual void          deactivate();
                          
                          
private:                  
    virtual void          draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void                  changeTexture(const Type buttonType);


private:
    Callback              mCallback;
    const sf::Texture&    mButtonsTexture;
    sf::Sprite            mSprite;
    sf::Text              mText;
    bool                  mIsToggle;
};


} // namespace gui
