#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

namespace test {
    class TestTransform : public Test
    {
    public:
        TestTransform();
        ~TestTransform();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
     private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader, m_Shader2;
        std::unique_ptr<Texture> m_Texture1, m_Texture2;

        glm::mat4 m_Proj, m_View;
        glm::mat4 m_Transform, m_Transform2;
        float m_Mix = 0.2;
    };
}