#pragma once

#include <glm/vec2.hpp>
#include <memory>

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"
#include "../../System/Timer.h"

namespace RenderEngine {
    class Sprite;
}

class Bullet;

class Tank : public IGameObject {
public:

    enum class EOrientation : uint8_t {
        Top,
        Bottom,
        Left,
        Right
    };

    Tank(const double maxVelocity,
         const glm::vec2& position,
         const glm::vec2& size,
         const float layer);

    void render() const override;
    void setOrientation(const EOrientation eOrientation);
    void update(const double delta) override;
    double getMaxVelocity() const { return m_maxVelocity; }
    void setVelocity(const double velocity) override;
    void fire();

private:
    EOrientation m_eOrientation;
    std::shared_ptr<Bullet> m_pCurrentBullet;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_top;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_bottom;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_left;
    std::shared_ptr<RenderEngine::Sprite> m_pSprite_right;
    RenderEngine::SpriteAnimator m_spriteAnimator_top;
    RenderEngine::SpriteAnimator m_spriteAnimator_bottom;
    RenderEngine::SpriteAnimator m_spriteAnimator_left;
    RenderEngine::SpriteAnimator m_spriteAnimator_right;

    std::shared_ptr<RenderEngine::Sprite> m_pSprite_respawn;
    RenderEngine::SpriteAnimator m_spriteAnimator_respawn;

    std::shared_ptr<RenderEngine::Sprite> m_pSprite_shield;
    RenderEngine::SpriteAnimator m_spriteAnimator_shield;

    Timer m_respawnTimer;
    Timer m_shieldTimer;

    double m_maxVelocity;
    bool m_isSpawning;
    bool m_hasShield;
};