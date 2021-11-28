#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>

#include <renderer/renderer.h>

#include <renderer/IndexBuffer.h>
#include <renderer/VertexBuffer.h>
#include <renderer/VertexArray.h>
#include <renderer/VertexBufferLayout.h>

#include <macros.h>

struct RendererData
{
    Vertex* verticies = nullptr;


    uint32_t whiteTexture = 0;

    uint32_t boundTextures[MaxTextureCount];
    uint32_t textureCount = 0;

    uint32_t quadCount = 0;
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;

    std::vector<uint32_t> loadedTextures;
};

namespace Renderer
{
    static VertexArray* vertexArray;
    static VertexBuffer* vertexBuffer;
    static IndexBuffer* indexBuffer;

    static RendererData* s_Data;

    bool Init()
    {
        s_Data = new RendererData;
        s_Data->verticies = new Vertex[MaxVertexCount];

        DEBUG_INFO("Creating vertex array");
        vertexArray = new VertexArray();

        DEBUG_INFO("Creating vertex buffer");
        vertexBuffer = new VertexBuffer(nullptr, sizeof(Vertex) * MaxVertexCount, GL_DYNAMIC_DRAW);

        // Vertex layout
        DEBUG_INFO("Setting up vertex layout");
        VertexBufferLayout layout;
        layout.Push(2);
        layout.Push(4);
        layout.Push(2);
        layout.Push(1);

        // Setting up the indicies for the vertex buffer
        DEBUG_INFO("Creating indicies");
        uint32_t* indicies = new uint32_t[MaxIndexCount];
        int offset = 0;
        for (int i = 0; i < MaxIndexCount; i += 6)
        {
            indicies[0 + i] = 0 + offset;
            indicies[1 + i] = 1 + offset;
            indicies[2 + i] = 2 + offset;

            indicies[3 + i] = 3 + offset;
            indicies[4 + i] = 2 + offset;
            indicies[5 + i] = 0 + offset;
            offset += 4;
        }
        DEBUG_INFO("Uploading indicies to index buffer");
        indexBuffer = new IndexBuffer(indicies, sizeof(uint32_t) * MaxIndexCount, GL_STATIC_DRAW);
        // Cleanup
        delete[] indicies;

        DEBUG_INFO("Adding buffers to vertex array");
        vertexArray->AddBuffer(*vertexBuffer, *indexBuffer, layout);

        // Creating the default white texture
        uint32_t texture = 0xffffffff;

        glGenTextures(1, &s_Data->whiteTexture);
        glBindTexture(GL_TEXTURE_2D, s_Data->whiteTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texture);

        s_Data->boundTextures[s_Data->textureCount++] = s_Data->whiteTexture;

        return true;
    }
    bool Shutdown()
    {
        DEBUG_INFO("Deleting textures!");
        glDeleteTextures(1, &s_Data->whiteTexture);

        for(int i = 0; i < s_Data->loadedTextures.size(); i++)
        {
            glDeleteTextures(1, &s_Data->loadedTextures[i]);
        }

        // Cleaning up buffers
        DEBUG_INFO("Deleting vertex buffer");
        delete vertexBuffer;
        DEBUG_INFO("Deleting index buffer");
        delete indexBuffer;
        DEBUG_INFO("Deleting vertex array");
        delete vertexArray;

        DEBUG_INFO("Deleting renderer data");
        delete s_Data;

        return true;
    }

    uint32_t LoadTexture(const char* path)
    {
        stbi_set_flip_vertically_on_load(true);

        int x, y, BPP;
        unsigned char* buffer = stbi_load(path, &x, &y, &BPP, 4);

        if (buffer == nullptr)
        {
            return 0;
        }

        uint32_t id;
        glGenTextures(1, &id);

        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(buffer);

        s_Data->loadedTextures.push_back(id);

        return id;
    }

    void BeginBatch()
    {
        s_Data->quadCount = 0;
        s_Data->vertexCount = 0;
        s_Data->indexCount = 0;

        // Set to 1 because the white texture is always bound
        s_Data->textureCount = 1;
    }

    void EndBatch()
    {
        vertexArray->Bind();
        vertexBuffer->BufferSubData(s_Data->verticies, s_Data->vertexCount * sizeof(Vertex));

        for (int i = 0; i < s_Data->textureCount; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, s_Data->boundTextures[i]);
        }

        glDrawElements(GL_TRIANGLES, s_Data->indexCount, GL_UNSIGNED_INT, 0);
    }

    void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, uint32_t textureID)
    {
        if (s_Data->quadCount >= MaxQuadCount || s_Data->textureCount >= MaxTextureCount)
        {
            EndBatch();
            BeginBatch();
        }

        uint32_t textureIndex = (uint32_t)-1;
        for (int i = 0; i < s_Data->textureCount; i++)
        {
            if (s_Data->boundTextures[i] == textureID)
            {
                textureIndex = i;
            }
        }
        if (textureIndex == (uint32_t)-1)
        {
            s_Data->boundTextures[s_Data->textureCount] = textureID;
            textureIndex = s_Data->textureCount++;;
        }

        std::array<Vertex, 4> verticies = CreateQuad(pos, size, color, textureIndex);

        s_Data->verticies[s_Data->vertexCount++] = verticies[0];
        s_Data->verticies[s_Data->vertexCount++] = verticies[1];
        s_Data->verticies[s_Data->vertexCount++] = verticies[2];
        s_Data->verticies[s_Data->vertexCount++] = verticies[3];

        s_Data->quadCount += 1;
        s_Data->indexCount += 6;
    }
    void DrawQuad(const glm::vec2& pos, const glm::vec2& size, uint32_t textureID)
    {
        DrawQuad(pos, size, {1.0f, 1.0f, 1.0f, 1.0f}, textureID);
    }
    void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad(pos, size, color, s_Data->whiteTexture);
    }

    std::array<Vertex, 4> CreateQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, uint32_t textureIndex)
    {
        std::array<Vertex, 4> verticies;

        verticies[0].pos = { pos.x - size.x / 2, pos.y - size.y / 2 };
        verticies[0].color = color;
        verticies[0].texCoord = { 0.0f, 0.0f };
        verticies[0].texIndex = static_cast<float>(textureIndex);

        verticies[1].pos = { pos.x + size.x / 2, pos.y - size.y / 2 };
        verticies[1].color = color;
        verticies[1].texCoord = { 1.0f, 0.0f };
        verticies[1].texIndex = static_cast<float>(textureIndex);

        verticies[2].pos = { pos.x + size.x / 2, pos.y + size.y / 2 };
        verticies[2].color = color;
        verticies[2].texCoord = { 1.0f, 1.0f };
        verticies[2].texIndex = static_cast<float>(textureIndex);

        verticies[3].pos = { pos.x - size.x / 2, pos.y + size.y / 2 };
        verticies[3].color = color;
        verticies[3].texCoord = { 0.0f, 1.0f };
        verticies[3].texIndex = static_cast<float>(textureIndex);


        return verticies;
    }
}