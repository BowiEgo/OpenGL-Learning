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
        
    private:
        Ref<VertexArray> m_VAO;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<Shader> m_Shader;
        Ref<Texture2D> m_Texture1, m_Texture2;
        
        glm::mat4 m_Proj, m_View;
        float m_Mix = 0.2;
    };
}