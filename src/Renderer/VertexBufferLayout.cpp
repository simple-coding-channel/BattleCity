#include "VertexBufferLayout.h"

namespace Renderer {
    VertexBufferLayout::VertexBufferLayout()
        : m_stride(0)
    {}

    void VertexBufferLayout::reserveElements(const size_t count)
    {
        m_layoutElments.reserve(count);
    }

    void VertexBufferLayout::addElementLayoutFloat(const unsigned int count, const bool normalized)
    {
        m_layoutElments.push_back({ static_cast<GLint>(count), GL_FLOAT, normalized, count * sizeof(GLfloat) });
        m_stride += m_layoutElments.back().size;
    }
}
