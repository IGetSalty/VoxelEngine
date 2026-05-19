#pragma once

class VertexBuffer
{
public:

    VertexBuffer(const void* data, unsigned int size);

    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    unsigned int getRendererID() const;

private:

    unsigned int m_RendererID;
};