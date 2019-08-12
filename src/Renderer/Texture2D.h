#pragma once

#include <glad/glad.h>
#include <string>

namespace Renderer {
    class Texture2D
    {
    public:
        Texture2D(const GLuint width, GLuint height,
                  const unsigned char* data,
                  const unsigned int channels = 4,
                  const GLenum filter = GL_LINEAR,
                  const GLenum wrapMode = GL_CLAMP_TO_EDGE);

        Texture2D() = delete;
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D& operator=(Texture2D&& texture2d);
        Texture2D(Texture2D&& texture2d);
        ~Texture2D();

        void bind() const;

    private:
        GLuint m_ID;
        GLenum m_mode;
        unsigned int m_width;
        unsigned int m_height;
    };
}