#include "Tank.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"
#include "Bullet.h"
#include "../../Physics/PhysicsEngine.h"

const std::string& Tank::getTankSpriteFromType(const ETankType eType)
{
    return TankTypeToSpriteString[static_cast<size_t>(eType)];
}

Tank::Tank(const Tank::ETankType eType,
           const bool bHasAI,
           const bool bShieldOnSpawn,
           const EOrientation eOrientation,
           const double maxVelocity,
           const glm::vec2& position,
           const glm::vec2& size,
           const float layer)
    : IGameObject(IGameObject::EObjectType::Tank, position, size, 0.f, layer)
    , m_eOrientation(eOrientation)
    , m_pCurrentBullet(std::make_shared<Bullet>(0.1, m_position + m_size / 4.f, m_size / 2.f, m_size, layer))
    , m_pSprite_top(ResourceManager::getSprite(getTankSpriteFromType(eType)    + "_top"))
    , m_pSprite_bottom(ResourceManager::getSprite(getTankSpriteFromType(eType) + "_bottom"))
    , m_pSprite_left(ResourceManager::getSprite(getTankSpriteFromType(eType)   + "_left"))
    , m_pSprite_right(ResourceManager::getSprite(getTankSpriteFromType(eType)  + "_right"))
    , m_spriteAnimator_top(m_pSprite_top)
    , m_spriteAnimator_bottom(m_pSprite_bottom)
    , m_spriteAnimator_left(m_pSprite_left)
    , m_spriteAnimator_right(m_pSprite_right)
    , m_pSprite_respawn(ResourceManager::getSprite("respawn"))
    , m_spriteAnimator_respawn(m_pSprite_respawn)
    , m_pSprite_shield(ResourceManager::getSprite("shield"))
    , m_spriteAnimator_shield(m_pSprite_shield)
    , m_maxVelocity(maxVelocity)
    , m_isSpawning(true)
    , m_hasShield(false)
    , m_bShieldOnSpawn(bShieldOnSpawn)
{
    setOrientation(m_eOrientation);

    m_respawnTimer.setCallback([&]()
    {
        m_isSpawning = false;
        if (m_pAIComponent)
        {
            m_velocity = m_maxVelocity;
        }
        if (m_bShieldOnSpawn)
        {
            m_hasShield = true;
            m_shieldTimer.start(2000);
        }
    });

    m_respawnTimer.start(1500);

    m_shieldTimer.setCallback([&]()
    {
        m_hasShield = false;
    });

    m_colliders.emplace_back(glm::vec2(0), m_size);

    m_pCurrentBullet->setOwner(this);
    Physics::PhysicsEngine::addDynamicGameObject(m_pCurrentBullet);

    if (bHasAI)
    {
        m_pAIComponent = std::make_unique<AIComponent>(this);
    }
}

void Tank::setVelocity(const double velocity)
{
    if (!m_isSpawning)
    {
        m_velocity = velocity;
    }
}

void Tank::render() const
{
    if (m_isSpawning)
    {
        m_pSprite_respawn->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_respawn.getCurrentFrame());
    }
    else
    {
        switch (m_eOrientation)
        {
        case Tank::EOrientation::Top:
            m_pSprite_top->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_top.getCurrentFrame());
            break;
        case Tank::EOrientation::Bottom:
            m_pSprite_bottom->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_bottom.getCurrentFrame());
            break;
        case Tank::EOrientation::Left:
            m_pSprite_left->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_left.getCurrentFrame());
            break;
        case Tank::EOrientation::Right:
            m_pSprite_right->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_right.getCurrentFrame());
            break;
        }

        if (m_hasShield)
        {
            m_pSprite_shield->render(m_position, m_size, m_rotation, m_layer + 0.1f, m_spriteAnimator_shield.getCurrentFrame());
        }
    }

    if (m_pCurrentBullet->isActive())
    {
        m_pCurrentBullet->render();
    }
}

void Tank::setOrientation(const EOrientation eOrientation)
{
    m_eOrientation = eOrientation;
    switch (m_eOrientation)
    {
    case Tank::EOrientation::Top:
        m_direction.x = 0.f;
        m_direction.y = 1.f;
        break;
    case Tank::EOrientation::Bottom:
        m_direction.x = 0.f;
        m_direction.y = -1.f;
        break;
    case Tank::EOrientation::Left:
        m_direction.x = -1.f;
        m_direction.y = 0.f;
        break;
    case Tank::EOrientation::Right:
        m_direction.x = 1.f;
        m_direction.y = 0.f;
        break;
    }
}

void Tank::update(const double delta)
{
    if (m_pCurrentBullet->isActive())
    {
        m_pCurrentBullet->update(delta);
    }

    if (m_isSpawning)
    {
        m_spriteAnimator_respawn.update(delta);
        m_respawnTimer.update(delta);
    }
    else
    {
        if (m_pAIComponent)
        {
            m_pAIComponent->update(delta);
        }

        if (m_hasShield)
        {
            m_spriteAnimator_shield.update(delta);
            m_shieldTimer.update(delta);
        }

        if (m_velocity > 0)
        {
            switch (m_eOrientation)
            {
            case Tank::EOrientation::Top:
                m_spriteAnimator_top.update(delta);
                break;
            case Tank::EOrientation::Bottom:
                m_spriteAnimator_bottom.update(delta);
                break;
            case Tank::EOrientation::Left:
                m_spriteAnimator_left.update(delta);
                break;
            case Tank::EOrientation::Right:
                m_spriteAnimator_right.update(delta);
                break;
            }
        }
    }
}

void Tank::fire()
{
    if (!m_isSpawning && !m_pCurrentBullet->isActive())
    {
        m_pCurrentBullet->fire(m_position + m_size / 4.f + m_size * m_direction / 4.f, m_direction);
    }
}
