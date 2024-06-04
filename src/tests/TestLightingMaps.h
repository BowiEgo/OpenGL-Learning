#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "FileSystem/FileSystem.h"

#include "Camera.h"

#include <memory>

namespace test {
    class TestLightingMaps : public Test
    {
    public:
        TestLightingMaps(GLFWwindow* window);
        ~TestLightingMaps();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(float deltaTime);
    private:
        std::unique_ptr<VertexArray> m_Obj_VAO, m_Light_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_ObjShader, m_LightShader;
        std::unique_ptr<Texture> m_DiffuseTexture, m_SpecularTexture, m_EmissionTexture;
        glm::vec3 m_ObjPositions, m_LightPosition;

        glm::mat4 m_Proj, m_View;
        std::unique_ptr<Camera> m_Camera;

        float m_MaterialShininess = 32.0f, m_MaterialEmmisionStength = 1.0f;

        float m_LightAmbient[3] = { 0.2f, 0.2f, 0.2f },
              m_LightDiffuse[3] = { 0.5f, 0.5f, 0.5f },
              m_LightSpecular[3] = { 1.0f, 1.0f, 1.0f };
    };
}