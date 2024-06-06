#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "FileSystem/FileSystem.h"

#include "Camera.h"

#include <memory>

namespace test {
    class TestLightCasters : public Test
    {
    public:
        TestLightCasters(GLFWwindow* window);
        ~TestLightCasters();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(float deltaTime);
        void SetCameraAspectRatio(const float aspectRatio) override;
        void EnableCameraControll() override;
        void DisableCameraControll() override;
    private:
        std::unique_ptr<VertexArray> m_Obj_VAO, m_LightCube_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_ObjShader, m_PointLightCubeShader, m_SpotLightCubeShader;
        std::unique_ptr<Texture> m_DiffuseTexture, m_SpecularTexture;
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

        std::unique_ptr<Camera> m_Camera;

        glm::vec3 m_LightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
        float m_MaterialShininess = 32.0f;

        float m_LightAmbient[3] = { 0.2f, 0.2f, 0.2f },
              m_LightDiffuse[3] = { 0.5f, 0.5f, 0.5f },
              m_LightSpecular[3] = { 1.0f, 1.0f, 1.0f };

        bool m_IsEnabled_DirectionalLight = true;
        float m_DirectionalLightDir[3] = { -0.2f, -1.0f, -0.3f };

        bool m_IsEnabled_PointLight = true;
        float m_PointLightPos[3] = { 1.2f, 1.0f, 2.0f };

        bool m_IsEnabled_SpotLight = true;
        float m_SpotLightPhi = 24.0f;
        float m_SpotLightCutOff = glm::cos(glm::radians(m_SpotLightPhi));
        float m_SpotLight_SoftEdge = 5.0f;
        float m_SpotLightOuterCutOff = glm::cos(glm::radians(m_SpotLightPhi + 5.0f));
    };
}