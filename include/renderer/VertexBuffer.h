#pragma once

#include <stdint.h>
#include <cstddef>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VertexBuffer
{
public:
    VertexBuffer(const void* data, size_t size, int usage = GL_DYNAMIC_DRAW);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
    
    void BufferSubData(const void* data, size_t size, int offset = 0);

    static inline const size_t GetObjectCount() { return s_ObjectCount; }
private:
    uint32_t m_RendererID;

    static size_t s_ObjectCount;
};