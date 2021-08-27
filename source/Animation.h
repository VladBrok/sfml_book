#pragma once
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Time.hpp"


class Animation: public sf::Drawable, public sf::Transformable
{
public:
                           Animation();
    explicit               Animation(const sf::Texture& texture);
                           
    void                   update(const sf::Time dt);
                           
    void                   setTexture(const sf::Texture& texture);
    const sf::Texture*     getTexture() const;
                    
    void                   setFrameSize(const sf::Vector2i& frameSize);
    const sf::Vector2i&    getFrameSize() const;
                    
    void                   setFramesCount(const std::size_t framesCount);
    std::size_t            getFramesCount() const;
                           
    void                   setDuration(const sf::Time duration);
    const sf::Time&        getDuration() const;
                           
    void                   setRepeating(const bool flag);
    bool                   isRepeating() const;
                           
    void                   restart();
    bool                   isFinished() const;
                           
    sf::FloatRect          getLocalBounds() const;
    sf::FloatRect          getGlobalBounds() const;
                           
                           
private:                   
    void                   draw(sf::RenderTarget& target, sf::RenderStates states) const;
                           
                           
private:                   
    sf::Sprite             mSprite;
    sf::Vector2i           mFrameSize;
    std::size_t            mFramesCount;
    std::size_t            mCurrentFrame;
    sf::Time               mDuration;
    sf::Time               mElapsedTime;
    bool                   mRepeat;
};



