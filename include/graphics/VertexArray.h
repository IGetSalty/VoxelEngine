#pragma once

class VertexArray
{
public:

    VertexArray();

    ~VertexArray();

    void bind() const;
    void unbind() const;

private:

    unsigned int m_RendererID;
};