#pragma once

class VertexArray
{
public:

    VertexArray();

    ~VertexArray();

    void bind() const;
    void unbind() const;

    unsigned int getRendererID() const;

private:

    unsigned int m_RendererID;
};