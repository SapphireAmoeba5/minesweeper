#pragma once

#include <renderer/renderer.h>
#include <renderer/Shader.h>
#include <Window.h>

#include <macros.h>

#include <thread>

#include <Random.h>

namespace unimportant
{
    uint32_t id;
    Window* window;

    static float previousTime;
    static float deltaTime;

    glm::vec2 size;
    glm::vec2 position;
    glm::vec2 velocity;

    void init(Window* swindow)
    {
        id = Renderer::LoadTexture("assets/textures/bird.png");
        window = swindow;
        previousTime = glfwGetTime();
        deltaTime = 0.0f;

        size = {480.0f / 5, 640.0f / 5};
        position = {100.0f, 100.0f};

        Random random(100, 250);
        velocity = {random(), random()}; 
    }

    void dothething()
    {
        using namespace std::chrono_literals;

        static bool pressed = false;
        static bool pressedPrevious = false;
        static bool show = false;
        pressed = glfwGetKey(window->GetWindow(), GLFW_KEY_B) == GLFW_PRESS;
        if(!pressedPrevious && pressed)
            show = !show;
        pressedPrevious = pressed;

        // Collides the the right edge of window
        if(position.x + size.x / 2 >= window->Size().x)
        {
            velocity.x = -velocity.x;
            position = {window->Size().x - size.x / 2, position.y};
        }
         // Collides with the left edge of window
        else if(position.x - size.x / 2 <= 0)
        {
            velocity.x = -velocity.x;
            position = {0.0f + size.x / 2, position.y};
        }
        // Collides with top edge of window
        else if(position.y + size.y / 2 >= window->Size().y)
        {
            velocity.y = -velocity.y;
            position = {position.x, window->Size().y - size.y / 2};
        }
        // Collides with the bottom edge of window
        else if(position.y - size.y / 2 <= 0) 
        {
            velocity.y = -velocity.y;
            position = {position.x, 0.0f + size.y / 2};
        }        
        
        position += velocity * deltaTime;

        if(show)
            Renderer::DrawQuad(position, size, id);
        

        deltaTime = glfwGetTime() - previousTime;
        previousTime = glfwGetTime();
    }
}