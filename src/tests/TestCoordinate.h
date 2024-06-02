#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

namespace test {
    class TestCoordinate : public Test
    {
    public:
        TestCoordinate();
        ~TestCoordinate();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
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
        float m_ViewX = 0.0f;
        float m_ViewY = 0.0f;
        float m_ViewZ = -3.0f;
    };
}