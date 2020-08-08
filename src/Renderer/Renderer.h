#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

#include <string>

namespace RenderEngine {
    class Renderer {
    public:
        static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader);
        static void setClearColor(const float r, const float g, const float b, const float a);
        static void setDepthTest(const bool enable);
        static void clear();
        static void setViewport(const unsigned int width, const unsigned int height, const unsigned int leftOffset = 0, const unsigned int bottomOffset = 0);

        static std::string getRendererStr();
        static std::string getVersionStr();
    };
}