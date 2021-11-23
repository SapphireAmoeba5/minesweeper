#pragma once

#include <stdint.h>
#include <cstddef>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

struct BufferLayoutData
{
    int size;
    uint32_t type;
    float normalized;
};

class VertexBufferLayout
{
public:
    VertexBufferLayout();
    ~VertexBufferLayout();

    static inline const size_t GetObjectCount() { return s_ObjectCount; }
    inline const size_t GetStride() const { return m_Stride; }
    inline const std::vector<BufferLayoutData>& GetLayout() const { return m_Layout; }
    void Push(int count);
private:
    size_t m_Stride;
    std::vector<BufferLayoutData> m_Layout;
    static size_t s_ObjectCount;
};