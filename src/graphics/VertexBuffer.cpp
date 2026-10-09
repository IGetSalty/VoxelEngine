#include "graphics/VertexBuffer.h"

#include <glad/glad.h>

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_RendererID);

}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int VertexBuffer::getRendererID() const {
    return m_RendererID;
}