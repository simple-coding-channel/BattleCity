#include "PhysicsEngine.h"

#include "../Game/GameObjects/IGameObject.h"
#include "../Game/GameStates/Level.h"

namespace Physics {

    std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::m_dynamicObjects;
    std::shared_ptr<Level> PhysicsEngine::m_pCurrentLevel;

    void PhysicsEngine::init()
    {

    }

    void PhysicsEngine::terminate()
    {
        m_dynamicObjects.clear();
        m_pCurrentLevel.reset();
    }

    void PhysicsEngine::setCurrentLevel(std::shared_ptr<Level> pLevel)
    {
        m_pCurrentLevel.swap(pLevel);
        m_dynamicObjects.clear();
        m_pCurrentLevel->initLevel();
    }

    void PhysicsEngine::update(const double delta)
    {
        calculateTargetPositions(m_dynamicObjects, delta);

        for (auto it1 = m_dynamicObjects.begin(); it1 != m_dynamicObjects.end(); )
        {
            auto& pObject1 = *it1;
            for (auto it2 = ++it1; it2 != m_dynamicObjects.end(); ++it2)
            {
                auto& pObject2 = *it2;
                if (pObject1->getOwner() == pObject2.get() || pObject2->getOwner() == pObject1.get())
                {
                    continue;
                }
                if (!hasPositionIntersection(pObject1, pObject1->getTargetPosition(),
                                             pObject2, pObject2->getTargetPosition()))
                {
                    continue;
                }

                if (hasPositionIntersection(pObject1, pObject1->getTargetPosition(),
                                            pObject2, pObject2->getCurrentPosition()))
                {
                    pObject1->getTargetPosition() = pObject1->getCurrentPosition();
                }

                if (hasPositionIntersection(pObject2, pObject2->getTargetPosition(),
                                            pObject1, pObject1->getCurrentPosition()))
                {
                    pObject2->getTargetPosition() = pObject2->getCurrentPosition();
                }
            }
        }

        updatePositions(m_dynamicObjects);
    }


    void PhysicsEngine::calculateTargetPositions(std::unordered_set<std::shared_ptr<IGameObject>>& dynamicObjects, const double delta)
    {
        for (auto& currentDynamicObject : dynamicObjects)
        {
            if (currentDynamicObject->getCurrentVelocity() > 0)
            {
                // align position to multiple of 4 pixels
                if (currentDynamicObject->getCurrentDirection().x != 0.f) // right and left
                {
                    currentDynamicObject->getTargetPosition() = glm::vec2(currentDynamicObject->getCurrentPosition().x, static_cast<unsigned int>(currentDynamicObject->getCurrentPosition().y / 4.f + 0.5f) * 4.f);
                }
                else if (currentDynamicObject->getCurrentDirection().y != 0.f) // top and bottom
                {
                    currentDynamicObject->getTargetPosition() = glm::vec2(static_cast<unsigned int>(currentDynamicObject->getCurrentPosition().x / 4.f + 0.5f) * 4.f, currentDynamicObject->getCurrentPosition().y);
                }

                const auto newPosition = currentDynamicObject->getTargetPosition() + currentDynamicObject->getCurrentDirection() * static_cast<float>(currentDynamicObject->getCurrentVelocity() * delta);
                std::vector<std::shared_ptr<IGameObject>> objectsToCheck = m_pCurrentLevel->getObjectsInArea(newPosition, newPosition + currentDynamicObject->getSize());


                const auto& colliders = currentDynamicObject->getColliders();
                bool hasCollision = false;

                ECollisionDirection dynamicObjectCollisionDirection = ECollisionDirection::Right;
                if (currentDynamicObject->getCurrentDirection().x < 0) dynamicObjectCollisionDirection = ECollisionDirection::Left;
                else if (currentDynamicObject->getCurrentDirection().y > 0) dynamicObjectCollisionDirection = ECollisionDirection::Top;
                else if (currentDynamicObject->getCurrentDirection().y < 0) dynamicObjectCollisionDirection = ECollisionDirection::Bottom;

                ECollisionDirection objectCollisionDirection = ECollisionDirection::Left;
                if (currentDynamicObject->getCurrentDirection().x < 0) objectCollisionDirection = ECollisionDirection::Right;
                else if (currentDynamicObject->getCurrentDirection().y > 0) objectCollisionDirection = ECollisionDirection::Bottom;
                else if (currentDynamicObject->getCurrentDirection().y < 0) objectCollisionDirection = ECollisionDirection::Top;

                for (const auto& currentDynamicObjectCollider : colliders)
                {
                    for (const auto& currentObjectToCheck : objectsToCheck)
                    {
                        const auto& collidersToCheck = currentObjectToCheck->getColliders();
                        if (currentObjectToCheck->collides(currentDynamicObject->getObjectType()) && !collidersToCheck.empty())
                        {
                            for (const auto& currentObjectCollider : currentObjectToCheck->getColliders())
                            {
                                if (currentObjectCollider.isActive && hasCollidersIntersection(currentDynamicObjectCollider, newPosition, currentObjectCollider, currentObjectToCheck->getCurrentPosition()))
                                {
                                    hasCollision = true;
                                    if (currentObjectCollider.onCollisionCallback)
                                    {
                                        currentObjectCollider.onCollisionCallback(*currentDynamicObject, objectCollisionDirection);
                                    }
                                    if (currentDynamicObjectCollider.onCollisionCallback)
                                    {
                                        currentDynamicObjectCollider.onCollisionCallback(*currentObjectToCheck, dynamicObjectCollisionDirection);
                                    }
                                }
                            }
                        }
                    }
                }


                if (!hasCollision)
                {
                    currentDynamicObject->getTargetPosition() = newPosition;
                }
                else
                {
                    // align position to multiple of 8 pixels
                    if (currentDynamicObject->getCurrentDirection().x != 0.f) // right and left
                    {
                        currentDynamicObject->getTargetPosition() = glm::vec2(static_cast<unsigned int>(currentDynamicObject->getTargetPosition().x / 4.f + 0.5f) * 4.f, currentDynamicObject->getTargetPosition().y);
                    }
                    else if (currentDynamicObject->getCurrentDirection().y != 0.f) // top and bottom
                    {
                        currentDynamicObject->getTargetPosition() = glm::vec2(currentDynamicObject->getTargetPosition().x, static_cast<unsigned int>(currentDynamicObject->getTargetPosition().y / 4.f + 0.5f) * 4.f);
                    }
                }
            }
        }
    }

    void PhysicsEngine::updatePositions(std::unordered_set<std::shared_ptr<IGameObject>>& dynamicObjects)
    {
        for (auto& currentDynamicObject : dynamicObjects)
        {
            currentDynamicObject->getCurrentPosition() = currentDynamicObject->getTargetPosition();
        }
    }


    void PhysicsEngine::addDynamicGameObject(std::shared_ptr<IGameObject> pGameObject)
    {
        m_dynamicObjects.insert(std::move(pGameObject));
    }

    bool PhysicsEngine::hasPositionIntersection(const std::shared_ptr<IGameObject>& pObject1, const glm::vec2& position1,
                                                const std::shared_ptr<IGameObject>& pObject2, const glm::vec2& position2)
    {
        const auto& currentObjectColliders = pObject1->getColliders();
        const auto& otherObjectColliders   = pObject2->getColliders();
        for (const auto& currentObjectCollider : currentObjectColliders)
        {
            for (const auto& otherObjectCollider : otherObjectColliders)
            {
                if (hasCollidersIntersection(currentObjectCollider, position1, otherObjectCollider, position2))
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool PhysicsEngine::hasCollidersIntersection(const Collider& collider1, const glm::vec2& position1,
                                                 const Collider& collider2, const glm::vec2& position2)
    {
            const glm::vec2 collider1_bottomLeft_world = collider1.boundingBox.bottomLeft + position1;
            const glm::vec2 collider1_topRight_world   = collider1.boundingBox.topRight   + position1;

            const glm::vec2 collider2_bottomLeft_world = collider2.boundingBox.bottomLeft + position2;
            const glm::vec2 collider2_topRight_world   = collider2.boundingBox.topRight   + position2;

            if (collider1_bottomLeft_world.x >= collider2_topRight_world.x)
            {
                return false;
            }
            if (collider1_topRight_world.x <= collider2_bottomLeft_world.x)
            {
                return false;
            }

            if (collider1_bottomLeft_world.y >= collider2_topRight_world.y)
            {
                return false;
            }
            if (collider1_topRight_world.y <= collider2_bottomLeft_world.y)
            {
                return false;
            }

            return true;
    }
}
