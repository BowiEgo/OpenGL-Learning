#pragma once

#include <functional>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace test {
    class Test
    {
    public:
        Test() : m_Window(nullptr) {}
        Test(GLFWwindow* window) : m_Window(window) {}
        virtual ~Test() {};

        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
    protected:
        GLFWwindow* m_Window;
    };

    class TestMenu : public Test
    {
    public:
        TestMenu(Test*& currentTestPointer, GLFWwindow* window);

        void OnImGuiRender() override;

        template<typename T>
        void RegisterTest(const std::string& name)
        {
            std::cout << "Registering test " << name << std::endl;

            m_Tests.push_back(std::make_pair(name, [this](){ return new T(m_Window); }));
        }
    private:
        Test*& m_CurrentTest;
        GLFWwindow* m_Window;
        std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
    };
}