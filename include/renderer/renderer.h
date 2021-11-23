#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

constexpr size_t MaxQuadCount = 100000;
constexpr size_t MaxVertexCount = MaxQuadCount * 4;
constexpr size_t MaxIndexCount = MaxQuadCount * 6;
constexpr uint32_t MaxTextureCount = 8;

struct Vertex
{
    glm::vec2 pos;
    glm::vec4 color;
    glm::vec2 texCoord;
    float texIndex;
};

namespace Renderer
{
    bool Init();
    bool Shutdown();

    uint32_t LoadTexture(const char* path);

    void BeginBatch();
    void EndBatch();

    void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, uint32_t textureID);
    void DrawQuad(const glm::vec2& pos, const glm::vec2& size, uint32_t textureID);
    void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);

    std::array<Vertex, 4> CreateQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, uint32_t textureIndex);
}