#include "BrickWall.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

#include <iostream>

BrickWall::EBrickState BrickWall::getBrickStateAfterCollision(const EBrickState currentState, const Physics::ECollisionDirection direction)
{
    switch (currentState)
    {
    case EBrickState::All:
        switch (direction)
        {
        case Physics::ECollisionDirection::Left:
            return EBrickState::Right;
        case Physics::ECollisionDirection::Right:
            return EBrickState::Left;
        case Physics::ECollisionDirection::Top:
            return EBrickState::Bottom;
        case Physics::ECollisionDirection::Bottom:
            return EBrickState::Top;
        }

    case EBrickState::Top:
        switch (direction)
        {
        case Physics::ECollisionDirection::Left:
            return EBrickState::TopRight;
        case Physics::ECollisionDirection::Right:
            return EBrickState::TopLeft;
        case Physics::ECollisionDirection::Top:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Bottom:
            return EBrickState::Destroyed;
        }

    case EBrickState::Bottom:
        switch (direction)
        {
        case Physics::ECollisionDirection::Left:
            return EBrickState::BottomRight;
        case Physics::ECollisionDirection::Right:
            return EBrickState::BottomLeft;
        case Physics::ECollisionDirection::Top:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Bottom:
            return EBrickState::Destroyed;
        }

    case EBrickState::Left:
        switch (direction)
        {
        case Physics::ECollisionDirection::Left:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Right:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Top:
            return EBrickState::BottomLeft;
        case Physics::ECollisionDirection::Bottom:
            return EBrickState::TopLeft;
        }

    case EBrickState::Right:
        switch (direction)
        {
        case Physics::ECollisionDirection::Left:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Right:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Top:
            return EBrickState::BottomRight;
        case Physics::ECollisionDirection::Bottom:
            return EBrickState::TopRight;
        }

    default:
        return EBrickState::Destroyed;
    }
}

Physics::AABB BrickWall::getAABBForBrickState(const EBrickLocation location, const EBrickState eBrickState, const glm::vec2& size)
{
    glm::vec2 blockOffset(0);
    switch (location)
    {
        case EBrickLocation::BottomLeft:
            break;
        case EBrickLocation::BottomRight:
            blockOffset.x += size.x / 2;
            break;
        case EBrickLocation::TopLeft:
            blockOffset.y += size.y / 2;
            break;
        case EBrickLocation::TopRight:
            blockOffset += size / 2.f;
            break;
    }

    glm::vec2 bottomLeft(0);
    glm::vec2 topRight(0);
    switch (eBrickState)
    {
        case EBrickState::All:
            topRight   = glm::vec2(size.x / 2, size.y / 2);
            break;
        case EBrickState::TopLeft:
            bottomLeft = glm::vec2(0, size.y / 4) ;
            topRight   = glm::vec2(size.x / 4, size.y / 2);
            break;
        case EBrickState::TopRight:
            bottomLeft = glm::vec2(size.x / 4, size.y / 4);
            topRight   = glm::vec2(size.x / 2, size.y / 2);
            break;
        case EBrickState::Top:
            bottomLeft = glm::vec2(0, size.y / 4);
            topRight   = glm::vec2(size.x / 2, size.y / 2);
            break;
        case EBrickState::BottomLeft:
            topRight   = glm::vec2(size.x / 4, size.y / 4);
            break;
        case EBrickState::Left:
            topRight   = glm::vec2(size.x / 4, size.y / 2);
            break;
        case EBrickState::TopRight_BottomLeft:
            topRight   = glm::vec2(size.x / 2, size.y / 2);
            break;
        case EBrickState::Top_BottomLeft:
            topRight   = glm::vec2(size.x / 2, size.y / 2);
            break;
        case EBrickState::BottomRight:
            bottomLeft = glm::vec2(size.x / 4, 0);
            topRight   = glm::vec2(size.x / 2, size.y / 4);
            break;
        case EBrickState::TopLeft_BottomRight:
            topRight   = glm::vec2(size.x / 2, size.y / 2);
            break;
        case EBrickState::Right:
            bottomLeft = glm::vec2(size.x / 4, 0) ;
            topRight   = glm::vec2(size.x / 2, size.y / 2);
            break;
        case EBrickState::Top_BottomRight:
            topRight   = glm::vec2(size.x / 2, size.y / 2);
            break;
        case EBrickState::Bottom:
            topRight   = glm::vec2(size.x / 2, size.y / 4);
            break;
        case EBrickState::TopLeft_Bottom:
            topRight   = glm::vec2(size.x / 2, size.y / 2);
            break;
        case EBrickState::TopRight_Bottom:
            topRight   = glm::vec2(size.x / 2, size.y / 2);
            break;
        case EBrickState::Destroyed:
            break;
    }

    return {bottomLeft + blockOffset, topRight + blockOffset};
}

void BrickWall::onCollisionCallback(const EBrickLocation location, const IGameObject& object, const Physics::ECollisionDirection direction)
{
    if (object.getObjectType() != IGameObject::EObjectType::Bullet) return;
    const EBrickState newBrickState = getBrickStateAfterCollision(m_eCurrentBrickState[static_cast<size_t>(location)], direction);
    m_eCurrentBrickState[static_cast<size_t>(location)] = newBrickState;
    if (newBrickState != EBrickState::Destroyed)
    {
        m_brickLocationToColliderMap[static_cast<size_t>(location)]->boundingBox = getAABBForBrickState(location, newBrickState, m_size);
    }
    else
    {
        m_brickLocationToColliderMap[static_cast<size_t>(location)]->isActive = false;
    }
};

BrickWall::BrickWall(const EBrickWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
    : IGameObject(IGameObject::EObjectType::BrickWall, position, size, rotation, layer)
    , m_eCurrentBrickState{ EBrickState::Destroyed,
                            EBrickState::Destroyed,
                            EBrickState::Destroyed,
                            EBrickState::Destroyed }
    , m_blockOffsets { glm::vec2(0, m_size.y / 2.f),
                       glm::vec2(m_size.x / 2.f, m_size.y / 2.f),
                       glm::vec2(0, 0),
                       glm::vec2(m_size.x / 2.f, 0) }
{
    m_sprites[static_cast<size_t>(EBrickState::All)]                 = ResourceManager::getSprite("brickWall_All");
    m_sprites[static_cast<size_t>(EBrickState::TopLeft)]             = ResourceManager::getSprite("brickWall_TopLeft");
    m_sprites[static_cast<size_t>(EBrickState::TopRight)]            = ResourceManager::getSprite("brickWall_TopRight");
    m_sprites[static_cast<size_t>(EBrickState::Top)]                 = ResourceManager::getSprite("brickWall_Top");
    m_sprites[static_cast<size_t>(EBrickState::BottomLeft)]          = ResourceManager::getSprite("brickWall_BottomLeft");
    m_sprites[static_cast<size_t>(EBrickState::Left)]                = ResourceManager::getSprite("brickWall_Left");
    m_sprites[static_cast<size_t>(EBrickState::TopRight_BottomLeft)] = ResourceManager::getSprite("brickWall_TopRight_BottomLeft");
    m_sprites[static_cast<size_t>(EBrickState::Top_BottomLeft)]      = ResourceManager::getSprite("brickWall_Top_BottomLeft");
    m_sprites[static_cast<size_t>(EBrickState::BottomRight)]         = ResourceManager::getSprite("brickWall_BottomRight");
    m_sprites[static_cast<size_t>(EBrickState::TopLeft_BottomRight)] = ResourceManager::getSprite("brickWall_TopLeft_BottomRight");
    m_sprites[static_cast<size_t>(EBrickState::Right)]               = ResourceManager::getSprite("brickWall_Right");
    m_sprites[static_cast<size_t>(EBrickState::Top_BottomRight)]     = ResourceManager::getSprite("brickWall_Top_BottomRight");
    m_sprites[static_cast<size_t>(EBrickState::Bottom)]              = ResourceManager::getSprite("brickWall_Bottom");
    m_sprites[static_cast<size_t>(EBrickState::TopLeft_Bottom)]      = ResourceManager::getSprite("brickWall_TopLeft_Bottom");
    m_sprites[static_cast<size_t>(EBrickState::TopRight_Bottom)]     = ResourceManager::getSprite("brickWall_TopRight_Bottom");


    auto onCollisionCallbackTopLeft = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
    {
        onCollisionCallback(EBrickLocation::TopLeft, object, direction);
    };
    auto onCollisionCallbackTopRight = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
    {
        onCollisionCallback(EBrickLocation::TopRight, object, direction);
    };
    auto onCollisionCallbackBottomLeft = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
    {
        onCollisionCallback(EBrickLocation::BottomLeft, object, direction);
    };
    auto onCollisionCallbackBottomRight = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
    {
        onCollisionCallback(EBrickLocation::BottomRight, object, direction);
    };

    m_brickLocationToColliderMap.fill(nullptr);
    m_colliders.reserve(4);
    switch (eBrickWallType)
    {
    case EBrickWallType::All:
        m_eCurrentBrickState.fill(EBrickState::All);
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::TopLeft)]     = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::TopLeft,     EBrickState::All, m_size), onCollisionCallbackTopLeft));
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::TopRight)]    = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::TopRight,    EBrickState::All, m_size), onCollisionCallbackTopRight));
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::BottomLeft)]  = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::BottomLeft,  EBrickState::All, m_size), onCollisionCallbackBottomLeft));
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::BottomRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::BottomRight, EBrickState::All, m_size), onCollisionCallbackBottomRight));
        break;
    case EBrickWallType::Top:
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopLeft)]  = EBrickState::All;
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickState::All;
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::TopLeft)]     = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::TopLeft,     EBrickState::All, m_size), onCollisionCallbackTopLeft));
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::TopRight)]    = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::TopRight,    EBrickState::All, m_size), onCollisionCallbackTopRight));
        break;
    case EBrickWallType::Bottom:
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomLeft)]  = EBrickState::All;
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::BottomLeft)]  = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::BottomLeft,  EBrickState::All, m_size), onCollisionCallbackBottomLeft));
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::BottomRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::BottomRight, EBrickState::All, m_size), onCollisionCallbackBottomRight));
        break;
    case EBrickWallType::Left:
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopLeft)]    = EBrickState::All;
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickState::All;
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::TopLeft)]     = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::TopLeft,     EBrickState::All, m_size), onCollisionCallbackTopLeft));
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::BottomLeft)]  = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::BottomLeft,  EBrickState::All, m_size), onCollisionCallbackBottomLeft));
        break;
    case EBrickWallType::Right:
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopRight)]    = EBrickState::All;
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::TopRight)]    = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::TopRight,    EBrickState::All, m_size), onCollisionCallbackTopRight));
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::BottomRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::BottomRight, EBrickState::All, m_size), onCollisionCallbackBottomRight));
        break;
    case EBrickWallType::TopLeft:
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopLeft)] = EBrickState::All;
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::TopLeft)]     = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::TopLeft,     EBrickState::All, m_size), onCollisionCallbackTopLeft));
        break;
    case EBrickWallType::TopRight:
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickState::All;
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::TopRight)]    = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::TopRight,    EBrickState::All, m_size), onCollisionCallbackTopRight));
        break;
    case EBrickWallType::BottomLeft:
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickState::All;
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::BottomLeft)]  = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::BottomLeft,  EBrickState::All, m_size), onCollisionCallbackBottomLeft));
        break;
    case EBrickWallType::BottomRight:
        m_eCurrentBrickState[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
        m_brickLocationToColliderMap[static_cast<size_t>(EBrickLocation::BottomRight)] = &(m_colliders.emplace_back(getAABBForBrickState(EBrickLocation::BottomRight, EBrickState::All, m_size), onCollisionCallbackBottomRight));
        break;
    }
}

void BrickWall::renderBrick(const EBrickLocation eBrickLocation) const
{
    const EBrickState state = m_eCurrentBrickState[static_cast<size_t>(eBrickLocation)];
    if (state != EBrickState::Destroyed)
    {
        m_sprites[static_cast<size_t>(state)]->render(m_position + m_blockOffsets[static_cast<size_t>(eBrickLocation)], m_size / 2.f, m_rotation, m_layer);
    }
}

void BrickWall::render() const
{
    renderBrick(EBrickLocation::TopLeft);
    renderBrick(EBrickLocation::TopRight);
    renderBrick(EBrickLocation::BottomLeft);
    renderBrick(EBrickLocation::BottomRight);
}
