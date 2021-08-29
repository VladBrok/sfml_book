#pragma once
#include "SFML/System/Time.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "StateStack.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Player.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"


class Application
{
public:
                        Application();
    void                run();
                        
                        
private:                
    void                processInput();
    void                update(const sf::Time dt);
    void                updateStatistics(const sf::Time dt);
    void                render();
                        
    void                registerStates();
                        
private:                
    const sf::Time      TIME_PER_FRAME;
                        
    sf::RenderWindow    mWindow;
    TextureHolder       mTextures;
    FontHolder          mFonts;
    Player              mPlayer;
    MusicPlayer         mMusicPlayer;
    SoundPlayer         mSoundPlayer;
                        
    StateStack          mStateStack;
                        
    sf::Text            mStatisticsText;
    sf::Time            mStatisticsUpdateTime;
    std::size_t         mStatisticsNumOfFrames;
};


