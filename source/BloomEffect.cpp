#include "BloomEffect.h"


BloomEffect::BloomEffect()
{
    mShaders.load(Shaders::BrightnessPass,   "Media/Shaders/Fullpass.vert", "Media/Shaders/Brightness.frag");
    mShaders.load(Shaders::DownSamplePass,   "Media/Shaders/Fullpass.vert", "Media/Shaders/DownSample.frag");
    mShaders.load(Shaders::GaussianBlurPass, "Media/Shaders/Fullpass.vert", "Media/Shaders/GuassianBlur.frag");
    mShaders.load(Shaders::AddPass,          "Media/Shaders/Fullpass.vert", "Media/Shaders/Add.frag");
}


void BloomEffect::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
    prepareTextures(input.getSize());

    filterBright(input, mBrightnessTexture);

    downsample(mBrightnessTexture, mFirstPassTextures[0]);
    blurMultipass(mFirstPassTextures);

    add(input, mFirstPassTextures[0], output);
}


void BloomEffect::prepareTextures(const sf::Vector2u& size)
{
    if (mBrightnessTexture.getSize() != size)
    {
        mBrightnessTexture.create(size.x, size.y);
        mBrightnessTexture.setSmooth(true);

        mFirstPassTextures[0].create(size.x / 2, size.y / 2);
        mFirstPassTextures[0].setSmooth(true);
        mFirstPassTextures[1].create(size.x / 2, size.y / 2);
        mFirstPassTextures[1].setSmooth(true);
    }
}


void BloomEffect::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output)
{
    sf::Shader& brightness = mShaders.get(Shaders::BrightnessPass);

    brightness.setUniform("source", input.getTexture());
    applyShader(brightness, output);
    output.display();
}


void BloomEffect::blurMultipass(RenderTextureArray& renderTextures)
{
    sf::Vector2u textureSize = renderTextures[0].getSize();
    blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
    blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
}


void BloomEffect::blur(const sf::RenderTexture& input, 
                       sf::RenderTexture& output, 
                       const sf::Vector2f& offsetFactor)
{
    sf::Shader& gaussianBlur = mShaders.get(Shaders::GaussianBlurPass);

    gaussianBlur.setUniform("source", input.getTexture());
    gaussianBlur.setUniform("offsetFactor", offsetFactor);
    applyShader(gaussianBlur, output);
    output.display();
}

void BloomEffect::downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
{
    sf::Shader& downSampler = mShaders.get(Shaders::DownSamplePass);

    downSampler.setUniform("source", input.getTexture());
    downSampler.setUniform("sourceSize", sf::Vector2f(input.getSize()));
    applyShader(downSampler, output);
}

void BloomEffect::add(const sf::RenderTexture& source, 
                      const sf::RenderTexture& bloom, 
                      sf::RenderTarget& output)
{
    sf::Shader& adder = mShaders.get(Shaders::AddPass);

    adder.setUniform("source", source.getTexture());
    adder.setUniform("bloom", bloom.getTexture());
    applyShader(adder, output);
}

