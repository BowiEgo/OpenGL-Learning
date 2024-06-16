#pragma once

#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"

#include "FileSystem/FileSystem.h"

#include "Camera.h"

#include <memory>
#include <vector>

namespace test {
    class TestLightCasters : public Test
    {
    public:
        TestLightCasters(GLFWwindow* window);
        ~TestLightCasters();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Ref<VertexArray> m_Obj_VAO, m_LightCube_VAO;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;
        Ref<Shader> m_ObjShader, m_PointLightCubeShader, m_SpotLightCubeShader;
        Ref<Texture2D> m_DiffuseTexture, m_SpecularTexture;
        std::vector<glm::vec3> m_ObjPositions = {
            glm::vec3( 0.0f,  0.0f,   0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f,  -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f,  -3.5f),
            glm::vec3(-1.7f,  3.0f,  -7.5f),
            glm::vec3( 1.3f, -2.0f,  -2.5f),
            glm::vec3( 1.5f,  2.0f,  -2.5f),
            glm::vec3( 1.5f,  0.2f,  -1.5f),
            glm::vec3(-1.3f,  1.0f,  -1.5f)  
        };

        glm::vec3 m_LightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
        float m_MaterialShininess = 32.0f;

        float m_LightAmbient[3] = { 0.2f, 0.2f, 0.2f },
              m_LightDiffuse[3] = { 0.5f, 0.5f, 0.5f },
              m_LightSpecular[3] = { 1.0f, 1.0f, 1.0f };

        bool m_IsEnabled_DirectionalLight = true;
        float m_DirectionalLightDir[3] = { -0.2f, -1.0f, -0.3f };

        bool m_IsEnabled_PointLight = true;
        std::vector<glm::vec3> m_PointLightPositions = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        };

        bool m_IsEnabled_SpotLight = true;
        float m_SpotLightPhi = 16.0f;
        float m_SpotLightCutOff = glm::cos(glm::radians(m_SpotLightPhi));
        float m_SpotLight_SoftEdge = 1.5f;
        float m_SpotLightOuterCutOff = glm::cos(glm::radians(m_SpotLightPhi + m_SpotLight_SoftEdge));
    };
}