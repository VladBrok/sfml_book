#include "Player.h"
#include "CommandQueue.h"
#include "Aircraft.h"


struct AircraftMover
{
    AircraftMover(const float velocityX, const float velocityY)
        : velocity(velocityX, velocityY)
    {
    }

    void operator() (Aircraft& aircraft, const sf::Time) const
    {
        aircraft.accelerate(velocity * aircraft.getMaxSpeed());
    }

    sf::Vector2f velocity;
};


Player::Player()
{
    initKeyBindings();
    initActionBindings();
}


void Player::handleEvent(const sf::Event& event, CommandQueue& commandQueue)
{
    if (event.type == sf::Event::KeyPressed)
    {
        auto foundKeyBinding = mKeyBindings.find(event.key.code);
        if (foundKeyBinding != mKeyBindings.end() && !isRealtimeAction(foundKeyBinding->second))
        {
            commandQueue.push(mActionBindings[foundKeyBinding->second]);
        }
    }
}


void Player::handleRealtimeInput(CommandQueue& commandQueue)
{
    for (const auto& keyBinding: mKeyBindings)
    {
        if (sf::Keyboard::isKeyPressed(keyBinding.first) && isRealtimeAction(keyBinding.second))
        {
            commandQueue.push(mActionBindings[keyBinding.second]);
        }
    }
}


void Player::assignKey(const Action action, const sf::Keyboard::Key key)
{
    // Remove all keys that already map to action
    for (auto keyBinding = mKeyBindings.begin(); keyBinding != mKeyBindings.end(); )
    {
        if (keyBinding->second == action)
        {
            keyBinding = mKeyBindings.erase(keyBinding);
        }
        else
        {
            ++keyBinding;
        }
    }

    // Insert new binding
    mKeyBindings[key] = action;
}


sf::Keyboard::Key Player::getAssignedKey(const Action action) const
{
    for (const auto& keyBinding: mKeyBindings)
    {
        if (keyBinding.second == action)
        {
            return keyBinding.first;
        }
    }
    return sf::Keyboard::Unknown;
}


void Player::initKeyBindings()
{
    mKeyBindings[sf::Keyboard::Left]  = MoveLeft;
    mKeyBindings[sf::Keyboard::Right] = MoveRight;
    mKeyBindings[sf::Keyboard::Up]    = MoveUp;
    mKeyBindings[sf::Keyboard::Down]  = MoveDown;
    mKeyBindings[sf::Keyboard::Space] = Fire;
    mKeyBindings[sf::Keyboard::M]     = LaunchMissile;
}


void Player::initActionBindings()
{
    // Movement
    mActionBindings[MoveLeft].action  = derivedAction<Aircraft>(AircraftMover(-1.f,  0.f));
    mActionBindings[MoveRight].action = derivedAction<Aircraft>(AircraftMover( 1.f,  0.f));
    mActionBindings[MoveUp].action    = derivedAction<Aircraft>(AircraftMover( 0.f, -1.f));
    mActionBindings[MoveDown].action  = derivedAction<Aircraft>(AircraftMover( 0.f,  1.f));

    // Combat
    mActionBindings[Fire].action = derivedAction<Aircraft>(
    [] (Aircraft& a, sf::Time) 
    { 
        a.fire(); 
    });
    mActionBindings[LaunchMissile].action = derivedAction<Aircraft>(
    [] (Aircraft& a, sf::Time)
    {
        a.launchMissile();
    });

    // Setting all command categories to PlayerAircraft
    for (auto& actionBinding: mActionBindings)
    {
        actionBinding.second.category = Category::PlayerAircraft;
    }
}


bool Player::isRealtimeAction(const Action action)
{
    switch (action)
    {
        case MoveLeft:
        case MoveRight:
        case MoveUp:
        case MoveDown:
        case Fire:
            return true;

        default:
            return false;
    }
}