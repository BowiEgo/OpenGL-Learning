#include "Input.h"

#include <GLFW/glfw3.h>

Input* Input::s_Instance = nullptr;
GLFWwindow* Input::m_Window = nullptr;
float Input::m_ScrollOffset = 0.0f;

void Input::SetMousePos(float x, float y)
{
    glfwSetCursorPos(m_Window, x, y);
}

void Input::InitMousePos()
{
    SetMousePos(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

void Input::DisableCursor()
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::EnableCursor()
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    InitMousePos();
}

bool Input::IsKeyPressedImpl(int keycode)
{
    auto state = glfwGetKey(m_Window, keycode);

    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressedImpl(int button)
{
    auto state = glfwGetKey(m_Window, button);

    return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePositionImpl()
{
    double xpos, ypos;
    glfwGetCursorPos(m_Window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
}

float Input::GetMouseXImpl()
{
    auto [x, y] = GetMousePositionImpl();

    return x;
}

float Input::GetMouseYImpl()
{
    auto [x, y] = GetMousePositionImpl();

    return y;
}

float Input::GetMouseScrollImpl()
{
    return m_ScrollOffset;
}
