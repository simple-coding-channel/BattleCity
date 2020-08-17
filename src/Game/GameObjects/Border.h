#pragma once

#include "IGameObject.h"

#include <array>
#include <memory>

namespace RenderEngine {
    class Sprite;
}

class Border : public IGameObject {
public:

    Border(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
    virtual void render() const override;

private:
    std::shared_ptr<RenderEngine::Sprite> m_sprite;
};