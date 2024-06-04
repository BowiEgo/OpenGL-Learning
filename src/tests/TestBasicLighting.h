#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "FileSystem/FileSystem.h"

#include "Camera.h"

#include <memory>

namespace test {
    class TestBasicLighting : public Test
    {
    public:
        TestBasicLighting(GLFWwindow* window);
        ~TestBasicLighting();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(float deltaTime);
    private:
        std::unique_ptr<VertexArray> m_Obj_VAO, m_Light_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_ObjShader, m_LightShader;
        glm::vec3 m_ObjPositions, m_LightPosition;

        glm::mat4 m_Proj, m_View;
        float m_Mix = 0.2;

        std::unique_ptr<Camera> m_Camera;

        float m_AmbientStrength = 0.1f, m_SpecularStrength = 0.5f;
        unsigned int m_Shininess = 32;
    };
}