#pragma once

#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"

namespace test {
    class TestTexture2DContainer : public Test
    {
    public:
        TestTexture2DContainer(GLFWwindow* window);
        ~TestTexture2DContainer();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetCameraAspectRatio(const float aspectRatio) override {};
    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture2D> m_Texture1, m_Texture2;
        
        glm::mat4 m_Proj, m_View;
        float m_Mix = 0.2;
    };
}