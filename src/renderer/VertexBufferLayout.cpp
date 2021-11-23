#include <renderer/VertexBufferLayout.h>

size_t VertexBufferLayout::s_ObjectCount = 0;

VertexBufferLayout::VertexBufferLayout()
    :m_Stride(0)
{
    s_ObjectCount++;
}

VertexBufferLayout::~VertexBufferLayout()
{
    s_ObjectCount--;
}

void VertexBufferLayout::Push(int count)
{
    m_Layout.push_back({
        count,
        GL_FLOAT,
        GL_FALSE,
    });

    m_Stride += count * sizeof(float);
}