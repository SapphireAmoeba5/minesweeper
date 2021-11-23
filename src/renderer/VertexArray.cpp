#include <renderer/VertexArray.h>

size_t VertexArray::s_ObjectCount = 0;

VertexArray::VertexArray()
{
    s_ObjectCount++;

    glGenVertexArrays(1, &m_RendererID);
    glBindVertexArray(m_RendererID);
}

VertexArray::~VertexArray()
{
    s_ObjectCount--;

    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const IndexBuffer& ib, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    ib.Bind();

    const auto& v = layout.GetLayout();

    // Offset is the byte offset to the current vertex attribute
    size_t offset = 0;
    for(int i = 0; i < v.size(); i++)
    {
        const auto& it = v[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, it.size, it.type, it.normalized, layout.GetStride(), (const void*)offset);
        offset += it.size * sizeof(float);
    }
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}