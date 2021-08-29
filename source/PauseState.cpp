#include "PauseState.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "Button.h"
#include "MusicPlayer.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RectangleShape.hpp"


PauseState::PauseState(StateStack& stack, Context context)
    : State(stack, context)
{
    sf::Font& font(context.fonts->get(Fonts::Main));
    const sf::Vector2f& windowSize(sf::Vector2f(context.window->getSize()));

    mPauseText.setFont(font);
	mPauseText.setString("Game Paused");	
	mPauseText.setCharacterSize(70);
	util::centerOrigin(mPauseText);
	mPauseText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

    auto returnButton = std::make_shared<gui::Button>(context);
	returnButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	returnButton->setText("Return");
	returnButton->setCallback([this] ()
	{
		requestStackPop();
	});

	auto backToMenuButton = std::make_shared<gui::Button>(context);
	backToMenuButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	backToMenuButton->setText("Back to menu");
	backToMenuButton->setCallback([this] ()
	{
		requestStackClear();
		requestStackPush(States::Menu);
	});

	mGuiContainer.pack(returnButton);
	mGuiContainer.pack(backToMenuButton);


    context.music->setPaused(true);
}


PauseState::~PauseState()
{
    getContext().music->setPaused(false);
}


bool PauseState::handleEvent(const sf::Event& event)
{
    mGuiContainer.handleEvent(event);
    return false;
}


bool PauseState::update(const sf::Time)
{
    return false;
}


void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mPauseText);
    window.draw(mGuiContainer);
}