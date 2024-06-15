#pragma once

#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"

namespace test {
    class TestCoordinate : public Test
    {
    public:
        TestCoordinate(GLFWwindow* window);
        ~TestCoordinate();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        
     private:
        Ref<VertexArray> m_VAO;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;
        Ref<Shader> m_Shader;
        Ref<Texture2D> m_Texture1, m_Texture2;
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