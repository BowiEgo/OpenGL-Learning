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
                m_CurrentTest->OnViewPortResize();
            }
        }
    }

    void Test::ProcessInput(float deltaTime)
    {
        if (m_Scene != nullptr)
        {
            Scene::GetCurrentCamera()->ProcessKeyboardMovement(deltaTime);
            Scene::GetCurrentCamera()->ProcessMouseMovement();
            Scene::GetCurrentCamera()->ProcessMouseScroll();
        }
    }

    void Test::EnableCameraControll()
    {
        if (m_Scene != nullptr)
        {
            Scene::GetCurrentCamera()->EnableControll();
        }
    }

    void Test::DisableCameraControll()
    {
        if (m_Scene != nullptr)
        {
            Scene::GetCurrentCamera()->DisableControll();
        }
    }

    void Test::OnViewPortResize()
    {
        if (m_Scene != nullptr)
        {
            if (Scene::GetCurrentCamera()->GetType() == CameraType::Perspective)
            {
                dynamic_cast<PerspectiveCamera*>(Scene::GetCurrentCamera())->SetAspectRatio(test::GetViewportSize().x / test::GetViewportSize().y);
            }
            Shader::UpdateMatricesProj(Scene::GetCurrentCamera()->GetProjMatrix());
        }
    }
}
