#include "Animation.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"


Animation::Animation()
    : mFrameSize(0, 0),
      mFramesCount(0),
      mCurrentFrame(0),
      mDuration(sf::Time::Zero),
      mElapsedTime(sf::Time::Zero),
      mRepeat(false)
{
}


Animation::Animation(const sf::Texture& texture)
    : mSprite(texture),
      mFrameSize(0, 0),
      mFramesCount(0),
      mCurrentFrame(0),
      mDuration(sf::Time::Zero),
      mElapsedTime(sf::Time::Zero),
      mRepeat(false)
{
}


void Animation::update(const sf::Time dt)
{
    sf::Time timePerFrame = mDuration / static_cast<float>(mFramesCount);
    mElapsedTime += dt;

    sf::Vector2i textureSize(mSprite.getTexture()->getSize());
    sf::IntRect textureRect(mSprite.getTextureRect());

    if (mCurrentFrame == 0)
    {
        textureRect = sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y);
    }


    while (mElapsedTime >= timePerFrame && (mCurrentFrame <= mFramesCount || mRepeat))
    {
        textureRect.left += textureRect.width;

        if (textureRect.left + textureRect.width > textureSize.x)
        {
            // Move the texture rectangle down one line
            textureRect.left = 0;
            textureRect.top += textureRect.height;
        }
        mElapsedTime -= timePerFrame;
        if (mRepeat)
        {
            mCurrentFrame = (mCurrentFrame + 1) % mFramesCount;
            if (mCurrentFrame == 0)
            {
                textureRect = sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y);
            }
        }
        else
        {
            ++mCurrentFrame;
        }
    }
    mSprite.setTextureRect(textureRect);
}


void Animation::setTexture(const sf::Texture& texture)
{
    mSprite.setTexture(texture);
}


const sf::Texture* Animation::getTexture() const
{
    return mSprite.getTexture();
}


void Animation::setFrameSize(const sf::Vector2i& frameSize)
{
    mFrameSize = frameSize;
}


const sf::Vector2i& Animation::getFrameSize() const
{
    return mFrameSize;
}


void Animation::setFramesCount(const std::size_t framesCount)
{
    mFramesCount = framesCount;
}


std::size_t Animation::getFramesCount() const
{
    return mFramesCount;
}


void Animation::setDuration(const sf::Time duration)
{
    mDuration = duration;
}


const sf::Time& Animation::getDuration() const
{
    return mDuration;
}


void Animation::setRepeating(const bool flag)
{
    mRepeat = flag;
}


bool Animation::isRepeating() const
{
    return mRepeat;
}


void Animation::restart()
{
    mCurrentFrame = 0;
}


bool Animation::isFinished() const
{
    return mCurrentFrame >= mFramesCount;
}


sf::FloatRect Animation::getLocalBounds() const
{
    return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}


sf::FloatRect Animation::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(mSprite, states);
}
