#include "BetonWall.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

BetonWall::BetonWall(const EBetonWallType eBetonWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
    : IGameObject(IGameObject::EObjectType::BetonWall, position, size, rotation, layer)
    , m_eCurrentBlockState{ EBlockState::Destroyed,
                            EBlockState::Destroyed,
                            EBlockState::Destroyed,
                            EBlockState::Destroyed }
    , m_sprite(ResourceManager::getSprite("betonWall"))
    , m_blockOffsets { glm::vec2(0, m_size.y / 2.f),
                       glm::vec2(m_size.x / 2.f, m_size.y / 2.f),
                       glm::vec2(0, 0),
                       glm::vec2(m_size.x / 2.f, 0) }
{
    switch (eBetonWallType)
    {
    case EBetonWallType::All:
        m_eCurrentBlockState.fill(EBlockState::Enabled);
        m_colliders.emplace_back(glm::vec2(0), m_size);
        break;
    case EBetonWallType::Top:
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)]  = EBlockState::Enabled;
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopRight)] = EBlockState::Enabled;
        m_colliders.emplace_back(glm::vec2(0, m_size.y / 2), m_size);
        break;
    case EBetonWallType::Bottom:
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)]  = EBlockState::Enabled;
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBlockState::Enabled;
        m_colliders.emplace_back(glm::vec2(0), glm::vec2(m_size.x, m_size.y / 2));
        break;
    case EBetonWallType::Left:
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)]    = EBlockState::Enabled;
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBlockState::Enabled;
        m_colliders.emplace_back(glm::vec2(0), glm::vec2(m_size.x / 2, m_size.y));
        break;
    case EBetonWallType::Right:
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopRight)]    = EBlockState::Enabled;
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBlockState::Enabled;
        m_colliders.emplace_back(glm::vec2(m_size.x / 2, 0), m_size);
        break;
    case EBetonWallType::TopLeft:
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBlockState::Enabled;
        m_colliders.emplace_back(glm::vec2(0, m_size.y / 2), glm::vec2(m_size.x / 2, m_size.y));
        break;
    case EBetonWallType::TopRight:
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopRight)] = EBlockState::Enabled;
        m_colliders.emplace_back(glm::vec2(m_size.x / 2, m_size.y / 2), m_size);
        break;
    case EBetonWallType::BottomLeft:
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBlockState::Enabled;
        m_colliders.emplace_back(glm::vec2(0), glm::vec2(m_size.x / 2, m_size.y / 2));
        break;
    case EBetonWallType::BottomRight:
        m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBlockState::Enabled;
        m_colliders.emplace_back(glm::vec2(m_size.x / 2, 0), glm::vec2(m_size.x, m_size.y / 2));
        break;
    }
}

void BetonWall::renderBlock(const EBlockLocation eBlockLocation) const
{
    const EBlockState state = m_eCurrentBlockState[static_cast<size_t>(eBlockLocation)];
    if (state != EBlockState::Destroyed)
    {
        m_sprite->render(m_position + m_blockOffsets[static_cast<size_t>(eBlockLocation)], m_size / 2.f, m_rotation, m_layer);
    }
}

void BetonWall::render() const
{
    renderBlock(EBlockLocation::TopLeft);
    renderBlock(EBlockLocation::TopRight);
    renderBlock(EBlockLocation::BottomLeft);
    renderBlock(EBlockLocation::BottomRight);
}

void BetonWall::update(const double delta)
{
}