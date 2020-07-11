#pragma once

#include <memory>

namespace RenderEngine {

    class Sprite;

    class SpriteAnimator {
    public:
        SpriteAnimator(std::shared_ptr<Sprite> pSprite);
        size_t getCurrentFrame() const { return m_currentFrame; }
        void update(const uint64_t delta);

    private:
        std::shared_ptr<Sprite> m_pSprite;
        size_t m_currentFrame;
        uint64_t m_currentFrameDuration;
        uint64_t m_currentAnimationTime;
    };

}