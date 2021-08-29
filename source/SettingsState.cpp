#include "SettingsState.h"
#include "Utility.h"
#include "SFML/Graphics/RenderWindow.hpp"


SettingsState::SettingsState(StateStack& stack, Context context)
    : State(stack, context)
{
    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    addButtonLabel(Player::MoveLeft,      100.f, "Move Left", context);
    addButtonLabel(Player::MoveRight,     150.f, "Move Right", context);
    addButtonLabel(Player::MoveUp,        200.f, "Move Up", context);
    addButtonLabel(Player::MoveDown,      250.f, "Move Down", context);
    addButtonLabel(Player::Fire,          300.f, "Fire", context);
    addButtonLabel(Player::LaunchMissile, 350.f, "Missile", context);

    updateLabels();

    auto backButton = std::make_shared<gui::Button>(context);
    backButton->setPosition(80.f, 420.f);
    backButton->setText("Back");
    backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));
    
    mGuiContainer.pack(backButton);
}


bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;
	
	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	for (std::size_t action = 0; action < Player::ActionCount; ++action)
	{
		if (mButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
				mButtons[action]->deactivate();
			}
			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	if (isKeyBinding)
		updateLabels();
	else
		mGuiContainer.handleEvent(event);

	return false;
}


bool SettingsState::update(const sf::Time)
{
    return true;
}


void SettingsState::draw()
{
    sf::RenderWindow& window(*getContext().window);

    window.draw(mBackgroundSprite);
    window.draw(mGuiContainer);
}


void SettingsState::updateLabels()
{
    Player& player = *getContext().player;

    for (std::size_t i = 0; i < mLabels.size(); ++i)
    {
        sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
        mLabels[i]->setText(util::keyToString(key));
    }
}


void SettingsState::addButtonLabel(Player::Action action, 
                                   const float y, 
                                   const std::string& text, 
                                   Context context)
{
    mButtons[action] = std::make_shared<gui::Button>(context);
	mButtons[action]->setPosition(80.f, y);
	mButtons[action]->setText(text);
	mButtons[action]->setToggle(true); 

    mLabels[action] = std::make_shared<gui::Label>("", *context.fonts);
    mLabels[action]->setPosition(300.f, y + 15.f);

    mGuiContainer.pack(mButtons[action]);
    mGuiContainer.pack(mLabels[action]);
}