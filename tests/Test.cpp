#include "Test.h"
#include "imgui.h"

#include "../src/Core/Shader/Shader.h"

namespace test
{
    glm::vec2 GetViewportSize()
    {
        return s_ViewportSize;
    }

    void UpdateViewportSize(float &width, float &height)
    {
        s_ViewportSize.x = width;
        s_ViewportSize.y = height;
    }

    TestMenu::TestMenu(Test*& currentTestPointer, GLFWwindow* window)
        : m_CurrentTest(currentTestPointer), m_Window(window)
    {
    }

    void TestMenu::OnImGuiRender()
    {
        for (auto& test : m_Tests)
        {
            if (ImGui::Button(test.first.c_str()))
            {
                m_CurrentTest = test.second();
                m_CurrentTest->OnViewPortResize(s_ViewportSize.x, s_ViewportSize.y);
            }
        }
    }

    void Test::OnViewPortResize(const float width, const float height)
    {
        if (m_Camera != nullptr)
        {
            m_Camera->SetAspectRatio(width / height);
            Shader::UpdateMatricesProj(m_Camera->GetProjMatrix());
        }
    }
}
