#include "MenuState.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "Button.h"
#include "MusicPlayer.h"
#include "SFML/Graphics/RenderWindow.hpp"


MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context)
{
    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));


	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<gui::Button>(context);
	playButton->setPosition(100, 250);
	playButton->setText("Play");
	playButton->setCallback([this] ()
	{
		requestStackPop();
		requestStackPush(States::Game);
	});

	auto settingsButton = std::make_shared<gui::Button>(context);
	settingsButton->setPosition(100, 300);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this] ()
	{
		requestStackPush(States::Settings);
	});

	auto exitButton = std::make_shared<gui::Button>(context);
	exitButton->setPosition(100, 350);
	exitButton->setText("Exit");
	exitButton->setCallback([this] ()
	{
		requestStackPop();
	});

	mGuiContainer.pack(playButton);
	mGuiContainer.pack(settingsButton);
	mGuiContainer.pack(exitButton);


    context.music->play(Music::MenuTheme);
}


bool MenuState::handleEvent(const sf::Event& event)
{
    mGuiContainer.handleEvent(event);
    return false;
}


bool MenuState::update(const sf::Time)
{
    return true;
}


void MenuState::draw()
{
    sf::RenderWindow& window(*getContext().window);

    window.setView(window.getDefaultView());
    window.draw(mBackgroundSprite);
    window.draw(mGuiContainer);
}
