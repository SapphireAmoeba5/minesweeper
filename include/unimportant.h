#pragma once

#include <renderer/renderer.h>
#include <renderer/Shader.h>
#include <Window.h>

#include <macros.h>

#include <thread>

#include <Random.h>

namespace unimportant
{
    struct BirdData
    {
        glm::vec2 position;
        glm::vec2 velocity;
    };

    uint32_t id;
    Window* window;

    static float previousTime;
    static float deltaTime;

    glm::vec2 size;
    glm::vec2 position;
    glm::vec2 velocity;

    std::vector<BirdData> birds;

    void init(Window* swindow)
    {
        id = Renderer::LoadTexture("assets/textures/bird.png");
        window = swindow;
        previousTime = glfwGetTime();
        deltaTime = 0.0f;

        size = {480.0f / 5, 640.0f / 5};
        position = {100.0f, 100.0f};

        Random random(100, 250);

        birds.push_back({{100.0f, 100.0f}, {random(), random()}});
    }

    void dothething()
    {
        using namespace std::chrono_literals;

        static bool Bpressed = false;
        static bool pressedPrevious = false;
        static bool show = false;
        Bpressed = glfwGetKey(window->GetWindow(), GLFW_KEY_B) == GLFW_PRESS;
        if(!pressedPrevious && Bpressed)
            show = !show;
        pressedPrevious = Bpressed;

        static bool mousePressed = false;
        static bool mousePressedPrev = mousePressed;


        mousePressed = glfwGetMouseButton(window->GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        glm::vec2 cursorPos = window->CursorPos();
        for(int i = 0; i < birds.size(); i++)
        {
            if(show && !mousePressedPrev && mousePressed &&
                cursorPos.x >= birds[i].position.x - size.x / 2 && cursorPos.x <= birds[i].position.x + size.x / 2 &&
                cursorPos.y >= birds[i].position.y - size.y / 2 && cursorPos.y <= birds[i].position.y + size.y / 2)
            {
                Random ran(5, 20);
                birds.insert(birds.begin() + i, {birds[i].position, birds[i].velocity + glm::vec2{ran(), ran()}});

                mousePressedPrev = mousePressed;
            }

            glm::vec2& position = birds[i].position;
            glm::vec2& velocity = birds[i].velocity;

                    // Collides the the right edge of window
            if(position.x + size.x / 2 >= window->Size().x)
            {
                velocity.x = -velocity.x;
                position = {window->Size().x - size.x / 2, position.y};
            }
            // Collides with the left edge of window
            if(position.x - size.x / 2 <= 0)
            {
                velocity.x = -velocity.x;
                position = {0.0f + size.x / 2, position.y};
            }
            // Collides with top edge of window
            if(position.y + size.y / 2 >= window->Size().y)
            {
                velocity.y = -velocity.y;
                position = {position.x, window->Size().y - size.y / 2};
            }
            // Collides with the bottom edge of window
            if(position.y - size.y / 2 <= 0) 
            {
                velocity.y = -velocity.y;
                position = {position.x, 0.0f + size.y / 2};
            }        

            birds[i].position += (birds[i].velocity * deltaTime);
            
            if(show)
                Renderer::DrawQuad(position, size, id);
        }

        mousePressedPrev = mousePressed;

        deltaTime = glfwGetTime() - previousTime;
        previousTime = glfwGetTime();
    }
}