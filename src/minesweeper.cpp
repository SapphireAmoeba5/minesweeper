#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <renderer/renderer.h>
#include <renderer/Shader.h>
#include <Window.h>

#include <board.h>

#include <macros.h>

#include "unimportant.h"

namespace Minesweeper
{
    void glfwMouseClickCallback(GLFWwindow* glfwWindow, int button, int state, int mods);
    void glfwKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
    static Window* window;
    static Shader* shader;

    glm::ivec2 boardSize = {16, 16};

    bool Init()
    {
        // Initializing GLFW
        DEBUG_INFO("Initializing OpenGL/GLFW");
        if (glfwInit() == GLFW_FALSE)
        {
            DEBUG_ERROR("Failed to initialize OpenGL/GLFW");
            return false;
        }


        Window::WindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        Window::WindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        Window::WindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        Window::WindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Creating the window and vertifying if it succeeded
        DEBUG_INFO("Creating GLFW window");
        window = new Window(900, 900, "Minesweeper");
        if (!window->IsValid())
        {
            DEBUG_ERROR("Failed to create OpenGL/GLFW window");
            glfwTerminate();
            return false;
        }
        window->Bind();

        glfwSetMouseButtonCallback(window->GetWindow(), glfwMouseClickCallback);
        glfwSetKeyCallback(window->GetWindow(), glfwKeyCallback);
        // Loading GLAD
        DEBUG_INFO("Initializing GLAD");
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            DEBUG_ERROR("Failed to initialize GLAD");
            return false;
        }

        DEBUG_INFO("Initializing renderer");
        Renderer::Init();

        DEBUG_INFO("Compiling shaders");
        shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

        // Initializing sampler2Ds in the shader
        DEBUG_INFO("Initializing samplers");
        int textures[32];
        for (int i = 0; i < 32; i++)
        {
            textures[i] = i;
        }
        shader->SetUniform1iv("u_Textures", 32, textures);

        DEBUG_INFO("Initializing MVP");
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 proj = glm::ortho(0.0f, (float)window->Size().x, 0.0f, (float)window->Size().y);
        shader->SetUniformMat4f("u_MVP", proj * view);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Board::Init(boardSize, window->Size());

        // Not important
        unimportant::init(window);

        return true;
    }

    bool Run()
    {
        size_t num = 0;
        while (!window->ShouldClose())
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            Renderer::BeginBatch();

            Board::Draw(*window);
            
            unimportant::dothething();
            
            Renderer::EndBatch();

            window->SwapBuffers();
            glfwPollEvents();
        }

        return true;
    }

    bool Shutdown()
    {
        DEBUG_INFO("Deleting shader");
        delete shader;

        DEBUG_INFO("Shutting down window");
        delete window;

        DEBUG_INFO("Shutting down renderer");
        Renderer::Shutdown();

        DEBUG_INFO("Destroying the board");
        Board::Shutdown();

        glfwTerminate();

        return true;
    }
}

void Minesweeper::glfwMouseClickCallback(GLFWwindow* glfwWindow, int button, int state, int mods)
{
    glm::vec2 cursorPos = window->CursorPos();

    if (state == GLFW_PRESS)
    {
        switch(button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                Board::Click(cursorPos, Board::Action::LeftClick);
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                Board::Click(cursorPos, Board::Action::RightClick);
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                if(Board::GetFailStatus() == true || Board::GetWinStatus() == true)
                {
                    Board::Shutdown();
                    Board::Init(boardSize, window->Size());
                }
                break;
        }
    }
}

void Minesweeper::glfwKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS && (((Board::GetFailStatus() == true || Board::GetWinStatus() == true) && key == GLFW_KEY_SPACE) || (key == GLFW_KEY_R && mods == GLFW_MOD_CONTROL)))
    {
        Board::Shutdown();
        Board::Init(boardSize, window->Size());
    }
}