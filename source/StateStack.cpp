#include "StateStack.h"


StateStack::StateStack(State::Context context)
    : mContext(context)
{
}


void StateStack::handleEvent(const sf::Event& event)
{
    // Iterate from top to bottom
    for (auto state = mStack.rbegin(); state != mStack.rend(); ++state)
    {
        if (!(*state)->handleEvent(event))
        {
            // Cease handling events of other states
            break;
        }
    }
    applyPendingChanges();
}


void StateStack::update(const sf::Time dt)
{
    // Iterate from top to bottom
    for (auto state = mStack.rbegin(); state != mStack.rend(); ++state)
    {
        if (!(*state)->update(dt))
        {
            // Cease updating other states
            break;
        }
    }
    applyPendingChanges();
}


void StateStack::draw()
{
    for (const auto& state: mStack)
    {
        state->draw();
    }
}

void StateStack::pushState(States::ID stateId)
{
    mPendingChanges.push_back(PendingChange(Action::Push, stateId));
}


void StateStack::popState()
{
    mPendingChanges.push_back(PendingChange(Action::Pop));
}


void StateStack::clearStates()
{
    mPendingChanges.push_back(PendingChange(Action::Clear));
}


bool StateStack::isEmpty() const
{
    return mStack.empty();
}

 
State::StatePtr StateStack::createState(States::ID stateId)
{
    auto found = mFactories.find(stateId);
    assert(found != mFactories.end());

    return found->second();
}


void StateStack::applyPendingChanges()
{
    for (const PendingChange& change: mPendingChanges)
    {
        switch (change.action)
        {
            case Push:
                mStack.push_back(createState(change.stateId));
                break;
            
            case Pop:
                mStack.pop_back();
                break;
            
            case Clear:
                mStack.clear();
                break;
        }
    }
    mPendingChanges.clear();
}


StateStack::PendingChange::PendingChange(Action action, States::ID stateId)
    : action(action),
      stateId(stateId)    
{
}
