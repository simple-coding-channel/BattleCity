#include "BrickWall.h"

#include "../../Renderer/Sprite.h"

BrickWall::BrickWall(const std::shared_ptr<RenderEngine::Sprite> pSprite, const glm::vec2& position, const glm::vec2& size, const float rotation)
    : IGameObject(position, size, rotation)
    , m_pCurrentSprite(std::move(pSprite))
{
}

void BrickWall::render() const
{
    m_pCurrentSprite->render(m_position, m_size, m_rotation);
}

void BrickWall::update(const uint64_t delta)
{
}