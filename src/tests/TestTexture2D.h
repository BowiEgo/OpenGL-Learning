#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture2D.h"

namespace test {
    class TestTexture2D : public Test
    {
    public:
        TestTexture2D(GLFWwindow* window);
        ~TestTexture2D();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetCameraAspectRatio(const float aspectRatio) override {};
     private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture2D> m_Texture;

        glm::mat4 m_Proj, m_View;
        glm::vec3 m_TranslationA, m_TranslationB;
    };
}