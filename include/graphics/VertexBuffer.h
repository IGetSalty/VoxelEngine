#pragma once

class VertexBuffer
{
public:

    VertexBuffer();

    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    unsigned int getRendererID() const;

private:

    unsigned int m_RendererID;
};