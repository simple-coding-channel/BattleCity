#pragma once

#include "IGameObject.h"

#include <array>
#include <memory>

namespace RenderEngine {
    class Sprite;
}

class Eagle : public IGameObject {
public:
    enum class EEagleState : uint8_t {
        Alive = 0,
        Dead
    };

    Eagle(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
    virtual void render() const override;
    void update(const double delta) override;

private:
    std::array<std::shared_ptr<RenderEngine::Sprite>, 2> m_sprite;
    EEagleState m_eCurrentState;
};