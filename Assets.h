#pragma once
#include "Animation.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>

typedef std::string AssetKey;

class Assets
{
    std::map<AssetKey, sf::Texture>     m_textures;
    std::map<AssetKey, Animation>       m_animations;
    std::map<AssetKey, sf::SoundBuffer> m_soundBuffers;
    std::map<AssetKey, sf::Font>        m_fonts;

public:

    sf::Texture& addTexture(AssetKey key, std::string path);
    Animation& addAnimation(AssetKey key, Animation animation);
    sf::SoundBuffer& addSoundBuffer(AssetKey key, std::string path);
    sf::Font& addFont(AssetKey key, std::string path);

    sf::Texture& getTexture(AssetKey key);
    Animation& getAnimation(AssetKey key);

    Assets(std::string path)
    {
        std::ifstream file(path);
        if (file.is_open())
        {
            std::string assetType;
            std::string assetKey;
            std::string assetPath;

            while (file >> assetType)
            {
                file >> assetKey;
                file >> assetPath;

                if (assetType == "Texture")
                {
                    addTexture(assetKey, assetPath);
                }
                else if (assetType == "Animation")
                {
                    size_t frameCount;
                    size_t frameDuration;
                    float scale;
                    float offsetX;
                    float offsetY;
                    bool repeat;
                    bool destroyOnEnd = false;

                    file >> frameCount;
                    file >> frameDuration;
                    file >> scale;
                    file >> offsetX;
                    file >> offsetY;
                    file >> repeat;

                    if (!repeat) file >> destroyOnEnd;

                    sf::Texture& texture = addTexture(assetKey, assetPath);
                    addAnimation(assetKey, Animation(texture, frameCount, frameDuration, scale, offsetX, offsetY, repeat, destroyOnEnd));
                }
                else if (assetType == "SoundBuffer")
                {
                    addSoundBuffer(assetKey, assetPath);
                }
                else if (assetType == "Font")
                {
                    addFont(assetKey, assetPath);
                }
            }
        }
        else std::cout << "Failed to open asset_config.txt\n";
    }
    sf::SoundBuffer& getSoundBuffer(AssetKey key);
    sf::Font& getFont(AssetKey key);
};