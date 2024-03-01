#include <iostream>
#include "Assets.h"

sf::Texture& Assets::addTexture(AssetKey key, std::string path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cout << "Failed to load texture\n";
    }
    m_textures[key] = texture;
    return m_textures[key];
}

Animation& Assets::addAnimation(AssetKey key, Animation animation)
{
    m_animations[key] = animation;
    return m_animations[key];
}

sf::SoundBuffer& Assets::addSoundBuffer(AssetKey key, std::string path)
{
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(path))
    {
        std::cout << "Failed to load sound buffer\n";
    }
    m_soundBuffers[key] = soundBuffer;
    return m_soundBuffers[key];
}

sf::Font& Assets::addFont(AssetKey key, std::string path)
{
    sf::Font font;
    if (!font.loadFromFile(path))
    {
        std::cout << "Failed to load font\n";
    }
    m_fonts[key] = font;
    return m_fonts[key];
}

sf::Texture& Assets::getTexture(AssetKey key)
{
    return m_textures[key];
}

Animation& Assets::getAnimation(AssetKey key)
{
    return m_animations[key];
}

sf::SoundBuffer& Assets::getSoundBuffer(AssetKey key)
{
    return m_soundBuffers[key];
}

sf::Font& Assets::getFont(AssetKey key)
{
    return m_fonts[key];
}
