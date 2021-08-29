#include "MusicPlayer.h"


MusicPlayer::MusicPlayer()
    : mVolume(45.f)
{
    mFilenames[Music::MenuTheme]    = "Media/Music/MenuTheme.ogg";
    mFilenames[Music::MissionTheme] = "Media/Music/MissionTheme.ogg";
}


void MusicPlayer::play(const Music::ID theme)
{
    std::string filename(mFilenames[theme]);

    if (!mMusic.openFromFile(filename))
    {
        throw std::runtime_error("ERROR in MusicPlayer::play: music " + filename + " could not be loaded.");
    }

    mMusic.setVolume(mVolume); // TODO: check reaction on invalid argument
    mMusic.setLoop(true);
    mMusic.play();
}


void MusicPlayer::stop()
{
    mMusic.stop();
}


void MusicPlayer::setPaused(const bool paused)
{
    paused ? mMusic.pause() : mMusic.play();
}


void MusicPlayer::setVolume(const float volume)
{
    mVolume = volume;
}