#include "graphics/VertexArray.h"

#include <glad/glad.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

unsigned int VertexArray::getRendererID() const {
    return m_RendererID;
}