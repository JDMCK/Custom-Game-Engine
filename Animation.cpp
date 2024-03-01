#include "Animation.h"

void Animation::update()
{
    m_currentFrame = (m_gameFrame / m_frameDuration) % m_frameCount;
    sf::IntRect rectangle(m_currentFrame * m_frameSize.x, 0, m_frameSize.x, m_frameSize.y);
    m_sprite.setTextureRect(rectangle);
    if (isRepeat() || !hasEnded())
    {
        m_gameFrame++;
    }
}

bool Animation::hasEnded() const
{
    return m_currentFrame == m_frameCount - 1;
}

sf::Sprite& Animation::getSprite()
{
    return m_sprite;
}

const Vec2& Animation::getSize() const
{
    return m_frameSize;
}

const bool Animation::isDestroyOnEnd() const
{
    return m_destroyOnEnd;
}

const bool Animation::isRepeat() const
{
    return m_repeat;
}
