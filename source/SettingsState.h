#pragma once
#include "State.h"
#include "Player.h"
#include "Container.h"
#include "Button.h"
#include "Label.h"
#include "SFML/Graphics/Sprite.hpp"
#include <array>


class SettingsState: public State
{
public:
                                                            SettingsState(StateStack& stack, 
                                                                          Context context);

    virtual bool                                            handleEvent(const sf::Event& event);
    virtual bool                                            update(const sf::Time dt);
    virtual void                                            draw();


private:
    void                                                    updateLabels();
    void                                                    addButtonLabel(Player::Action action, 
                                                                           const float y, 
                                                                           const std::string& text, 
                                                                           Context context); 


private:
    sf::Sprite                                              mBackgroundSprite;
    gui::Container                                          mGuiContainer;
    std::array<gui::Button::ButtonPtr, Player::ActionCount> mButtons;
    std::array<gui::Label::LabelPtr, Player::ActionCount>   mLabels;
};

