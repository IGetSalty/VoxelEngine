#pragma once

#include <glad/glad.h>

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    void bind(GLenum target = GL_ARRAY_BUFFER) const;
    void unbind(GLenum target = GL_ARRAY_BUFFER) const;

    unsigned int getRendererID() const;

private:
    unsigned int m_RendererID;
};