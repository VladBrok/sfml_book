#include "Application.h"
#include "Utility.h"
#include "SFML/Window/Event.hpp"
#include "TitleState.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "SettingsState.h"

#include <iostream>


Application::Application()
    : TIME_PER_FRAME(sf::seconds(1.f / 60.f)),
      mWindow(sf::VideoMode(1024, 768), "Sky storm", sf::Style::Close),
      mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer)),
      mStatisticsNumOfFrames(0)
{
    mWindow.setFramerateLimit(60);

    mTextures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
    mTextures.load(Textures::Buttons,     "Media/Textures/Buttons.png");

    mFonts.load(Fonts::Main, "Media/Sansation.ttf");
    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setCharacterSize(15);
    mStatisticsText.setPosition(10.f, 10.f);

    registerStates();
    mStateStack.pushState(States::Title);

}


void Application::run()
{
    sf::Clock timer;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        const sf::Time dt = timer.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > TIME_PER_FRAME)
        {
            timeSinceLastUpdate -= TIME_PER_FRAME;

            processInput();
            update(TIME_PER_FRAME);

            // If stack of states is empty - close the window
            if (mStateStack.isEmpty())
            {
                mWindow.close();
            }
        }
           
        updateStatistics(dt);
        render();
    }
}


void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
        {
            mWindow.close();
        }
    }
}


void Application::update(const sf::Time dt)
{
    mStateStack.update(dt);
}


void Application::updateStatistics(const sf::Time dt)
{
    mStatisticsUpdateTime += dt;
    mStatisticsNumOfFrames += 1;

    if (mStatisticsUpdateTime >= sf::seconds(1.f))
    {
        mStatisticsText.setString("FPS: " + util::toString(mStatisticsNumOfFrames));

        mStatisticsUpdateTime -= sf::seconds(1.f);
        mStatisticsNumOfFrames = 0;
    }
}


void Application::render()
{
    mWindow.clear();
    
    mStateStack.draw();
    mWindow.draw(mStatisticsText);

    mWindow.display();
}


void Application::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
    mStateStack.registerState<SettingsState>(States::Settings);
}
