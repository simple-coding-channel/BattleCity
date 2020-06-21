#include "IGameObject.h"

IGameObject::IGameObject(const glm::vec2& position, const glm::vec2& size, const float rotation)
    : m_position(position)
    , m_size(size)
    , m_rotation(rotation)
{
}

IGameObject::~IGameObject()
{
}