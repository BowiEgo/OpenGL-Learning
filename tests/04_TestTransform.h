#pragma once

#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"

namespace test {
    class TestTransform : public Test
    {
    public:
        TestTransform(GLFWwindow* window);
        ~TestTransform();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetCameraAspectRatio(const float aspectRatio) override {};
     private:
        Ref<VertexArray> m_VAO;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<Shader> m_Shader, m_Shader2;
        Ref<Texture2D> m_Texture1, m_Texture2;

        glm::mat4 m_Proj, m_View;
        glm::mat4 m_Transform, m_Transform2;
        float m_Mix = 0.2;
    };
}