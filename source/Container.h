#pragma once
#include "Component.h"
#include <vector>


namespace gui
{


class Container: public Component
{
public:
    typedef std::shared_ptr<Container> ContainerPtr;


public:
                                         Container();
                                         
    virtual void                         handleEvent(const sf::Event& event);
    virtual bool                         isSelectable() const;
    void                                 pack(Component::ComponentPtr component);
                                         
                                         
private:                                 
    virtual void                         draw(sf::RenderTarget& target, sf::RenderStates states) const;
                                         
    bool                                 hasSelection() const;
    void                                 select(std::size_t index);
    void                                 selectNext();
    void                                 selectPrevious();


private:
    std::vector<Component::ComponentPtr> mChildren;
    int                                  mSelectedChild;
};


} // namespace gui

