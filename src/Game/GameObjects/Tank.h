#pragma once

#include <glm/vec2.hpp>
#include <memory>

#include "IGameObject.h"

namespace RenderEngine {
    class AnimatedSprite;
}

class Tank : public IGameObject {
public:

    enum class EOrientation {
        Top,
        Bottom,
        Left,
        Right
    };

    Tank(std::shared_ptr<RenderEngine::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position, const glm::vec2& size);

    void render() const override;
    void setOrientation(const EOrientation eOrientation);
    void move(const bool move);
    void update(const uint64_t delta) override;

private:
    EOrientation m_eOrientation;
    std::shared_ptr<RenderEngine::AnimatedSprite> m_pSprite;
    bool m_move;
    float m_velocity;
    glm::vec2 m_moveOffset;
};