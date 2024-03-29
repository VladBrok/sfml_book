#pragma once

namespace sf
{
    class Texture;
    class Font;
    class Shader;
    class SoundBuffer;
}


namespace Textures
{
    enum ID
    {
        Entities,
        Jungle,
        TitleScreen,
        Buttons,
        Explosion,
        Particle,
        FinishLine,
    };
}


namespace Fonts
{
    enum ID
    {
        Main,
    };
}


namespace Shaders
{
    enum ID
    {
        BrightnessPass,  
        DownSamplePass,  
        GaussianBlurPass,
        AddPass,         
    };
}


namespace Music
{
    enum ID
    {
        MenuTheme,
        MissionTheme,
    };
}


namespace SoundEffects
{
    enum ID
    {
        AlliedGunfire,
        EnemyGunfire,
        Explosion1,
        Explosion2,
        LaunchMissile,
        CollectPickup,
        Button,
    };
}


template <typename Resource, typename Identifier>
class ResourceHolder;


typedef ResourceHolder<sf::Texture, Textures::ID>         TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>               FontHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID>           ShaderHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffects::ID> SoundBufferHolder;
