#include "Bullet.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Bullet::Bullet(const double velocity,
               const glm::vec2& position,
               const glm::vec2& size,
               const glm::vec2& explosionSize,
               const float layer)
    : IGameObject(IGameObject::EObjectType::Bullet, position, size, 0.f, layer)
    , m_explosionSize(explosionSize)
    , m_explosionOffset((m_explosionSize - m_size) / 2.f)
    , m_pSprite_top(ResourceManager::getSprite("bullet_Top"))
    , m_pSprite_bottom(ResourceManager::getSprite("bullet_Bottom"))
    , m_pSprite_left(ResourceManager::getSprite("bullet_Left"))
    , m_pSprite_right(ResourceManager::getSprite("bullet_Right"))
    , m_pSprite_explosion(ResourceManager::getSprite("explosion"))
    , m_spriteAnimator_explosion(m_pSprite_explosion)
    , m_eOrientation(EOrientation::Top)
    , m_maxVelocity(velocity)
    , m_isActive(false)
    , m_isExplosion(false)
{
    auto onCollisionCallback = [&](const IGameObject& object, const Physics::ECollisionDirection)
    {
        setVelocity(0);
        m_isExplosion = true;
        m_explosionTimer.start(m_spriteAnimator_explosion.getTotalDuration());
    };
    m_colliders.emplace_back(glm::vec2(0), m_size, onCollisionCallback);

    m_explosionTimer.setCallback([&]()
        {
            m_isExplosion = false;
            m_isActive = false;
            m_spriteAnimator_explosion.reset();
        }
    );
}

void Bullet::render() const
{
    if (m_isExplosion)
    {
        switch (m_eOrientation)
        {
        case EOrientation::Top:
            m_pSprite_explosion->render(m_position - m_explosionOffset + glm::vec2(0, m_size.y / 2.f), m_explosionSize, m_rotation, m_layer + 0.1f, m_spriteAnimator_explosion.getCurrentFrame());
            break;
        case EOrientation::Bottom:
            m_pSprite_explosion->render(m_position - m_explosionOffset - glm::vec2(0, m_size.y / 2.f), m_explosionSize, m_rotation, m_layer + 0.1f, m_spriteAnimator_explosion.getCurrentFrame());
            break;
        case EOrientation::Left:
            m_pSprite_explosion->render(m_position - m_explosionOffset - glm::vec2(m_size.x / 2.f, 0), m_explosionSize, m_rotation, m_layer + 0.1f, m_spriteAnimator_explosion.getCurrentFrame());
            break;
        case EOrientation::Right:
            m_pSprite_explosion->render(m_position - m_explosionOffset + glm::vec2(m_size.x / 2.f, 0), m_explosionSize, m_rotation, m_layer + 0.1f, m_spriteAnimator_explosion.getCurrentFrame());
            break;
        }
    }
    else if (m_isActive)
    {
        switch (m_eOrientation)
        {
        case EOrientation::Top:
            m_pSprite_top->render(m_position, m_size, m_rotation, m_layer);
            break;
        case EOrientation::Bottom:
            m_pSprite_bottom->render(m_position, m_size, m_rotation, m_layer);
            break;
        case EOrientation::Left:
            m_pSprite_left->render(m_position, m_size, m_rotation, m_layer);
            break;
        case EOrientation::Right:
            m_pSprite_right->render(m_position, m_size, m_rotation, m_layer);
            break;
        }
    }

}

void Bullet::update(const double delta)
{
    if (m_isExplosion)
    {
        m_spriteAnimator_explosion.update(delta);
        m_explosionTimer.update(delta);
    }
}

void Bullet::fire(const glm::vec2& position, const glm::vec2& direction)
{
    m_position = position;
    m_direction = direction;
    if (m_direction.x == 0.f)
    {
        m_eOrientation = (m_direction.y < 0) ? EOrientation::Bottom : EOrientation::Top;
    }
    else
    {
        m_eOrientation = (m_direction.x < 0) ? EOrientation::Left : EOrientation::Right;
    }
    m_isActive = true;
    setVelocity(m_maxVelocity);
}
