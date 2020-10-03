#pragma once
#include <unordered_set>
#include <memory>

#include <glm/vec2.hpp>

class IGameObject;
class Level;

namespace Physics {
    struct AABB {
        AABB(const glm::vec2& _bottomLeft, const glm::vec2 _topRight)
            : bottomLeft(_bottomLeft)
            , topRight(_topRight)
        {}
        glm::vec2 bottomLeft;
        glm::vec2 topRight;
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

        static bool hasIntersection(const std::vector<AABB>& colliders1, const glm::vec2& position1,
                                    const std::vector<AABB>& colliders2, const glm::vec2& position2);
    };
}

