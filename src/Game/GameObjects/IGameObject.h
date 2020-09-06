#pragma once

#include <glm/vec2.hpp>

class IGameObject {
public:
    IGameObject(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
    virtual void render() const = 0;
    virtual void update(const double delta) {};
    virtual ~IGameObject();

protected:
    glm::vec2 m_position;
    glm::vec2 m_size;
    float m_rotation;
    float m_layer;
};