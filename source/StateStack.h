#pragma once
#include "State.h"
#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/Time.hpp"

#include <list>
#include <functional>
#include <unordered_map>
#include <cassert>


class StateStack: private sf::NonCopyable
{
public:
    enum Action
    {
        Push,
        Pop,
        Clear
    };


public:
    explicit           StateStack(State::Context context);

    template <typename T>
    void               registerState(States::ID stateId);
                       
    void               handleEvent(const sf::Event& event);
    void               update(const sf::Time dt);
    void               draw();
                       
    void               pushState(States::ID stateId);
    void               popState();
    void               clearStates();
                       
    bool               isEmpty() const;


private:
    State::StatePtr    createState(States::ID stateId);
    void               applyPendingChanges();


private:
    struct PendingChange
    {
        explicit       PendingChange(Action action, States::ID stateId = States::None);
                       
        Action         action;
        States::ID     stateId;
    };


private:
    std::list<State::StatePtr>                                       mStack;
    std::list<PendingChange>                                         mPendingChanges;

    State::Context                                                   mContext;
    std::unordered_map<States::ID, std::function<State::StatePtr()>> mFactories;
};


template <typename StateType>
void StateStack::registerState(States::ID stateId)
{
    assert(stateId >= 0 && stateId <= States::StatesCount);

    mFactories[stateId] = [this] ()
    {
        return State::StatePtr(new StateType(*this, mContext));
    };
}

