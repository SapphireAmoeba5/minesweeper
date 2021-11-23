#include <Window.h>

#include <macros.h>

size_t Window::s_ObjectCount = 0;

Window::Window(glm::ivec2 size, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
    Window(size.x, size.y, title, monitor, share);
}

Window::Window(int posX, int posY, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
    s_ObjectCount++;

    m_Window = glfwCreateWindow(posX, posY, title, monitor, share);
    glfwMakeContextCurrent(m_Window);
}

Window::~Window()
{
    s_ObjectCount--;
    glfwDestroyWindow(m_Window);
}

void Window::Bind() const
{
    glfwMakeContextCurrent(m_Window);
}

void Window::Unbind() const
{
    glfwMakeContextCurrent(NULL);
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

void Window::SetTitle(const char* title)
{
    m_Title = title;
    glfwSetWindowTitle(m_Window, title);
}

const glm::ivec2 Window::Size()
{
    int x, y;
    glfwGetWindowSize(m_Window, &x, &y);
    return { x,y };
}

void Window::Size(int* width, int* height)
{
    glfwGetWindowSize(m_Window, width, height);
}

const glm::vec2 Window::CursorPos()
{
    double x,y;
    glfwGetCursorPos(m_Window, &x, &y);

    y = (y - Size().y) * -1;

    return {x,y};
}

void Window::CursorPos(double* x, double* y)
{
    glfwGetCursorPos(m_Window, x, y);

    *y = ((*y) - Size().y) * -1;
}

const glm::ivec2 Window::Position()
{
    int x, y;
    glfwGetWindowPos(m_Window, &x, &y);
    return {x,y};
}

void Window::Position(int* x, int* y)
{
    glfwGetWindowPos(m_Window, x, y);
}