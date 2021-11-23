#include <renderer/IndexBuffer.h>

size_t IndexBuffer::s_ObjectCount = 0;

IndexBuffer::IndexBuffer(const void* data, size_t size, int usage /*= GL_DYNAMIC_DRAW*/)
{
    s_ObjectCount++;

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

IndexBuffer::~IndexBuffer()
{
    s_ObjectCount--;
    
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}