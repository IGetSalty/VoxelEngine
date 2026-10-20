#include "graphics/VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_RendererID);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::bind(GLenum target) const
{
    glBindBuffer(target, m_RendererID);
}

void VertexBuffer::unbind(GLenum target) const
{
    glBindBuffer(target, 0);
}

unsigned int VertexBuffer::getRendererID() const
{
    return m_RendererID;
}