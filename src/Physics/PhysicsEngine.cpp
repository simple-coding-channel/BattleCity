#include "PhysicsEngine.h"

#include "../Game/GameObjects/IGameObject.h"

std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::m_dynamicObjects;

void PhysicsEngine::init()
{

}

void PhysicsEngine::terminate()
{
    m_dynamicObjects.clear();
}

void PhysicsEngine::update(const double delta)
{
    for (auto& currentObject : m_dynamicObjects)
    {
        if (currentObject->getCurrentVelocity() > 0)
        {
            currentObject->getCurrentPosition() += currentObject->getCurrentDirection() * static_cast<float>(currentObject->getCurrentVelocity() * delta);
        }
    }
}

void PhysicsEngine::addDynamicGameObject(std::shared_ptr<IGameObject> pGameObject)
{
    m_dynamicObjects.insert(std::move(pGameObject));
}