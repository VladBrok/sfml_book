#include "Component.h"


namespace gui
{


Component::Component()
    : mIsSelected(false),
      mIsActive(false)
{
}


bool Component::isSelected() const
{
    return mIsSelected;
}


void Component::select()
{
    mIsSelected = true;
}


void Component::deselect()
{
    mIsSelected = false;
}


bool Component::isActive()
{
    return mIsActive;
}


void Component::activate()
{
    mIsActive = true;
}


void Component::deactivate()
{
    mIsActive = false;
}


} // namespace gui
