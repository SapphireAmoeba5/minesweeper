#pragma once

#include <stdint.h>
#include <cstddef>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddBuffer(const VertexBuffer& vb, const IndexBuffer& ib, const VertexBufferLayout& layout);

    static inline const size_t GetObjectCount() { return s_ObjectCount; }
private:
    uint32_t m_RendererID;

    static size_t s_ObjectCount;
};