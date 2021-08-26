#pragma once
#include "SFML/System/NonCopyable.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"

#include <memory>
namespace sf
{
    class Event;
}


namespace gui
{


class Component: public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
public:
    typedef std::shared_ptr<Component> ComponentPtr;


                    Component();
    virtual         ~Component() {};

    virtual void    handleEvent(const sf::Event& event) = 0;
                    
    virtual bool    isSelectable() const = 0;
    bool            isSelected() const;
    virtual void    select();
    virtual void    deselect();
                    
    virtual bool    isActive();
    virtual void    activate();
    virtual void    deactivate();


private:
    bool            mIsSelected;
    bool            mIsActive;
};


} // namespace gui
