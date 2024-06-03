#include "Test.h"
#include "imgui.h"

namespace test
{
    TestMenu::TestMenu(Test*& currentTestPointer, GLFWwindow* window)
        : m_CurrentTest(currentTestPointer), m_Window(window)
    {
    }

    void TestMenu::OnImGuiRender()
    {
        for (auto& test : m_Tests)
        {
            if (ImGui::Button(test.first.c_str()))
                m_CurrentTest = test.second();
        }
    }
}
