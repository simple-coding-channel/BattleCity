#include "IndexBuffer.h"

namespace Renderer {
    IndexBuffer::IndexBuffer()
        : m_id(0)
    {
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& indexBuffer) noexcept
    {
        m_id = indexBuffer.m_id;
        indexBuffer.m_id = 0;
        return *this;
    }

    IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept
    {
        m_id = indexBuffer.m_id;
        indexBuffer.m_id = 0;
    }

    void IndexBuffer::init(const void* data, const unsigned int size)
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}