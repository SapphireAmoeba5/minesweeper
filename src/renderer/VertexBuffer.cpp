#include <renderer/VertexBuffer.h>

size_t VertexBuffer::s_ObjectCount = 0;

VertexBuffer::VertexBuffer(const void* data, size_t size, int usage /*= GL_DYNAMIC_DRAW*/)
{
    s_ObjectCount++;

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer()
{
    s_ObjectCount--;

    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BufferSubData(const void* data, size_t size, int offset /*= 0*/)
{
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}