#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "FileSystem/FileSystem.h"

namespace test {
    class TestCamera : public Test
    {
    public:
        TestCamera(GLFWwindow* window);
        ~TestCamera();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(float deltaTime);
    private:
        unsigned int VAO;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture1, m_Texture2;
        std::vector<glm::vec3> m_CubePositions;

        glm::mat4 m_Proj, m_View;
        float m_Mix = 0.2;
        float m_FOV = 45.0f;
        float m_AspectRatio = 960.0f / 540.0f;
        glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 m_CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f), m_CameraRight, m_CameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
        float m_Pitch = 0.0f, m_Yaw = 0.0f;
        float m_LastMouseX = 960.f / 2, m_LastMouseY = 540.0f / 2, m_LastMouseScroll = 0.0f;
    };
}