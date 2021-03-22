#pragma once
#include <unordered_set>
#include <memory>
#include <vector>
#include <functional>

#include <glm/vec2.hpp>

class IGameObject;
class Level;

namespace Physics {

    enum class ECollisionDirection : uint8_t {
        Top,
        Bottom,
        Left,
        Right
    };

    struct AABB {
        AABB(const glm::vec2& _bottomLeft, const glm::vec2 _topRight)
            : bottomLeft(_bottomLeft)
            , topRight(_topRight)
        {}
        glm::vec2 bottomLeft;
        glm::vec2 topRight;
    };

    struct Collider {
        Collider(const glm::vec2& _bottomLeft, const glm::vec2 _topRight, std::function<void(const IGameObject&, const ECollisionDirection)> _onCollisionCallback = {})
            : boundingBox(_bottomLeft, _topRight)
            , isActive(true)
            , onCollisionCallback(_onCollisionCallback)
        {}

        Collider(const AABB& _boundingBox, std::function<void(const IGameObject&, const ECollisionDirection)> _onCollisionCallback = {})
            : boundingBox(_boundingBox)
            , isActive(true)
            , onCollisionCallback(_onCollisionCallback)
        {}

        AABB boundingBox;
        bool isActive;
        std::function<void(const IGameObject&, const ECollisionDirection)> onCollisionCallback;
    };



    class PhysicsEngine {
    public:
        ~PhysicsEngine() = delete;
        PhysicsEngine() = delete;
        PhysicsEngine(const PhysicsEngine&) = delete;
        PhysicsEngine& operator=(const PhysicsEngine&) = delete;
        PhysicsEngine& operator=(PhysicsEngine&&) = delete;
        PhysicsEngine(PhysicsEngine&&) = delete;

        static void init();
        static void terminate();

        static void update(const double delta);
        static void addDynamicGameObject(std::shared_ptr<IGameObject> pGameObject);
        static void setCurrentLevel(std::shared_ptr<Level> pLevel);

    private:
        static std::unordered_set<std::shared_ptr<IGameObject>> m_dynamicObjects;
        static std::shared_ptr<Level> m_pCurrentLevel;
        static bool hasCollidersIntersection(const Collider& collider1, const glm::vec2& position1,
                                             const Collider& collider2, const glm::vec2& position2);
        static bool hasPositionIntersection(const std::shared_ptr<IGameObject>& pObject1, const glm::vec2& position1,
                                            const std::shared_ptr<IGameObject>& pObject2, const glm::vec2& position2);

        static void calculateTargetPositions(std::unordered_set<std::shared_ptr<IGameObject>>& dynamicObjects, const double delta);
        static void updatePositions(std::unordered_set<std::shared_ptr<IGameObject>>& dynamicObjects);
    };
}

