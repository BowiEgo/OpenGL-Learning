#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture2D.h"

namespace test {
    class TestCoordinate : public Test
    {
    public:
        TestCoordinate(GLFWwindow* window);
        ~TestCoordinate();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetCameraAspectRatio(const float aspectRatio) override;
     private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture2D> m_Texture1, m_Texture2;
        std::vector<glm::vec3> m_CubePositions;

        glm::mat4 m_Proj, m_View;
        float m_Mix = 0.2;
        float m_FOV = 45.0f;
        float m_AspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
        float m_ViewX = 0.0f;
        float m_ViewY = 0.0f;
        float m_ViewZ = -3.0f;
    };
}