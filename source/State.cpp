#include "State.h"
#include "StateStack.h"
#include "Player.h"
#include "SFML/Graphics/RenderWindow.hpp"


State::Context::Context(sf::RenderWindow& window,
                        TextureHolder& textures,
                        FontHolder& fonts,
                        Player& player,
                        MusicPlayer& musicPlayer,
                        SoundPlayer& soundPlayer)
    : window(&window),
      textures(&textures),
      fonts(&fonts),
      player(&player),
      music(&musicPlayer),
      sounds(&soundPlayer)
{
}


State::State(StateStack& stack, Context context)
    : mStack(&stack),
      mContext(context)
{
}


void State::requestStackPush(States::ID stateId)
{
    mStack->pushState(stateId);
}


void State::requestStackPop()
{
    mStack->popState();
}


void State::requestStackClear()
{
    mStack->clearStates();
}
                  

const State::Context& State::getContext() const
{
    return mContext;
}