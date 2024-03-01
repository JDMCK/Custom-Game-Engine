#pragma once

#include <SFML/Graphics.hpp>
#include "Vec2.h"

class Animation
{
    bool   m_repeat = false;
    bool   m_destroyOnEnd = false;
    size_t m_currentFrame = 0;
    size_t m_frameCount = 0;
    size_t m_gameFrame = 0;
    size_t m_frameDuration = 0;

    sf::Sprite m_sprite;
    Vec2       m_frameSize = Vec2::ZERO;

    float offsetX = 0.f;
    float offsetY = 0.f;

public:

    Animation() {};

    Animation(const sf::Texture& texture, size_t frameCount, size_t frameDuration, float scale = 1,
              float offsetX = 0, float offsetY = 0, bool repeat = false, bool destroyOnEnd = false) :
        m_sprite(texture),
        m_frameCount(frameCount),
        m_currentFrame(0),
        m_gameFrame(0),
        m_frameDuration(frameDuration),
        offsetX(offsetX),
        offsetY(offsetY),
        m_repeat(repeat),
        m_destroyOnEnd(destroyOnEnd)
    {
        m_frameSize = Vec2(texture.getSize().x / frameCount, texture.getSize().y);
        m_sprite.setOrigin((m_frameSize.x + offsetX) / 2, (m_frameSize.y + offsetY) / 2);
        m_sprite.setTextureRect(sf::IntRect(m_currentFrame * m_frameSize.x, 0, m_frameSize.x, m_frameSize.y));
        m_sprite.setScale(scale, scale);
    }

    void update();
    bool hasEnded() const;
    sf::Sprite& getSprite();
    const Vec2& getSize() const;
    const bool isDestroyOnEnd() const;
    const bool isRepeat() const;
};