#include "Renderer.h"

namespace RenderEngine {

    void Renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader)
    {
        shader.use();
        vertexArray.bind();
        indexBuffer.bind();

        glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::setClearColor(const float r, const float g, const float b, const float a)
    {
        glClearColor(r, g, b, a);
    }

    void Renderer::setDepthTest(const bool enable)
    {
        if (enable)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void Renderer::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::setViewport(const unsigned int width, const unsigned int height, const unsigned int leftOffset, const unsigned int bottomOffset)
    {
        glViewport(leftOffset, bottomOffset, width, height);
    }

    std::string Renderer::getRendererStr()
    {
        return (const char*)glGetString(GL_RENDERER);
    }

    std::string Renderer::getVersionStr()
    {
        return (const char*)glGetString(GL_VERSION);
    }
}