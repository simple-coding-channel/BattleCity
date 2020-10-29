#pragma once

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"

#include <array>
#include <memory>

namespace RenderEngine {
    class Sprite;
}

class Water : public IGameObject {
public:

    enum class EBlockLocation : uint8_t {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    Water(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
    virtual void render() const override;
    void update(const double delta) override;
    virtual bool collides(const EObjectType objectType) override;

private:
    void renderBlock(const EBlockLocation eBlockLocation) const;

    std::shared_ptr<RenderEngine::Sprite> m_sprite;
    RenderEngine::SpriteAnimator m_spriteAnimator;
    std::array<glm::vec2, 4> m_blockOffsets;
};