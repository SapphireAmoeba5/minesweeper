#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <macros.h>


class Window
{
public: // Constructor and destructor
    Window(glm::ivec2 size, const char* title, GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);
    Window(int posX, int posY, const char* title, GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);
    ~Window();

public: // Public methods
    void Bind() const;
    void Unbind() const;

    bool ShouldClose();
    void SwapBuffers();
public: // Setters
    void SetTitle(const char* title);

public: // Getters
    inline GLFWwindow* GetWindow() const { return m_Window; }
    inline const char* const Title() const { return m_Title; }
    inline const bool IsValid() const { return m_Window != nullptr; }
    static inline const size_t GetObjectCount() { return s_ObjectCount; }

    const glm::ivec2 Size();
    void Size(int* width, int* height);

    const glm::vec2 CursorPos();
    void CursorPos(double* x, double* y);

    const glm::ivec2 Position();
    void Position(int* x, int* y);

public: // Public static methods
    static void WindowHint(int hint, int value)
    {
        glfwWindowHint(hint, value);
    }
    
private:
    const char* m_Title;

    GLFWwindow* m_Window;

    static size_t s_ObjectCount;
};