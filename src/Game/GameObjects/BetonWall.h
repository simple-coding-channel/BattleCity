#pragma once

#include "IGameObject.h"

#include <array>
#include <memory>

namespace RenderEngine {
    class Sprite;
}

class BetonWall : public IGameObject {
public:

    enum class EBetonWallType {
        All,
        Top,
        Bottom,
        Left,
        Right,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    enum class EBlockState {
        Enabled = 0,
        Destroyed
    };

    enum class EBlockLocation {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    BetonWall(const EBetonWallType eBetonWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
    virtual void render() const override;
    virtual void update(const double delta) override;

private:
    void renderBlock(const EBlockLocation eBlockLocation) const;

    std::array<EBlockState, 4> m_eCurrentBlockState;
    std::shared_ptr<RenderEngine::Sprite> m_sprite;
    std::array<glm::vec2, 4> m_blockOffsets;
};