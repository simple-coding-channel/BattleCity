#include "IGameObject.h"

IGameObject::IGameObject(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
    : m_position(position)
    , m_size(size)
    , m_rotation(rotation)
    , m_layer(layer)
{
}

IGameObject::~IGameObject()
{
}