#pragma once

#include "pch.h"

#include <GLFW/glfw3.h>

class Input
{
public:
    Input(GLFWwindow* window) {
        m_Window = window;
        glfwSetScrollCallback(window, ScrollCallback);
    };
    ~Input();

    inline static void Create(GLFWwindow* window) { s_Instance = new Input(window); }

    inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

    inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
    inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
    inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
    inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
    inline static float GetMouseScroll() { return s_Instance->GetMouseScrollImpl(); }
    inline static float GetMouseOffsetX() { return s_Instance->m_OffsetX; };
    inline static float GetMouseOffsetY() { return s_Instance->m_OffsetY; };
    static void SetMousePos(float x, float y);
    static void InitMousePos();

    static void DisableCursor();
    static void EnableCursor();

protected:
    static bool IsKeyPressedImpl(int keycode);

    static bool IsMouseButtonPressedImpl(int button);
    static std::pair<float, float> GetMousePositionImpl();
    static float GetMouseXImpl();
    static float GetMouseYImpl();
    static float GetMouseScrollImpl();
private:
    static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
    {
        s_Instance->m_ScrollOffset = static_cast<float>(yOffset);
    }
private:
    static Input* s_Instance;
    static GLFWwindow* m_Window;
    static float m_ScrollOffset;
    float m_LastMouseX = 0.0f, m_LastMouseY = 0.0f;
    float m_OffsetX = 0.0f, m_OffsetY = 0.0f;
};