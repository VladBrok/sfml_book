#include "GameState.h"
#include "MusicPlayer.h"
#include "SFML/Graphics/RenderWindow.hpp"


GameState::GameState(StateStack& stack, Context context)
    : State(stack, context),
      mPlayer(context.player),
      mWorld(*context.window, *context.fonts, *context.sounds)
{
    context.music->play(Music::MissionTheme);
}


bool GameState::handleEvent(const sf::Event& event)
{
    mPlayer->handleEvent(event, mWorld.getCommandQueue());

    if (event.type == sf::Event::KeyPressed
            && event.key.code == sf::Keyboard::Escape)
    {
        requestStackPush(States::Pause);
    }

    return true;
}


bool GameState::update(const sf::Time dt)
{
    mWorld.update(dt);

    mPlayer->handleRealtimeInput(mWorld.getCommandQueue());

    return true;
}


void GameState::draw()
{
    mWorld.draw();
}