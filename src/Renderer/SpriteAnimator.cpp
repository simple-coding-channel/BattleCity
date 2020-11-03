#include "SpriteAnimator.h"

#include "Sprite.h"

namespace RenderEngine {
    SpriteAnimator::SpriteAnimator(std::shared_ptr<Sprite> pSprite)
        : m_pSprite(std::move(pSprite))
        , m_currentFrame(0)
        , m_currentFrameDuration(m_pSprite->getFrameDuration(0))
        , m_currentAnimationTime(0)
        , m_totalDuration(0)
    {
        for (size_t currentFrameId = 0; currentFrameId < m_pSprite->getFramesCount(); ++currentFrameId)
        {
            m_totalDuration += m_pSprite->getFrameDuration(currentFrameId);
        }
    }

    void SpriteAnimator::update(const double delta)
    {
        m_currentAnimationTime += delta;
        while (m_currentAnimationTime >= m_currentFrameDuration)
        {
            m_currentAnimationTime -= m_currentFrameDuration;
            ++m_currentFrame;
            if (m_currentFrame == m_pSprite->getFramesCount())
            {
                m_currentFrame = 0;
            }
            m_currentFrameDuration = m_pSprite->getFrameDuration(m_currentFrame);
        }
    }

    void SpriteAnimator::reset()
    {
        m_currentFrame = 0;
        m_currentFrameDuration = m_pSprite->getFrameDuration(0);
        m_currentAnimationTime = 0;
    }
}
