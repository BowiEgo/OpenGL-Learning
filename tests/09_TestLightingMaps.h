#pragma once

#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"

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
        
        void EnableCameraControll() override;
        void DisableCameraControll() override;
    private:
        Ref<VertexArray> m_Obj_VAO, m_Light_VAO;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;
        Ref<Shader> m_ObjShader, m_LightCubeShader;
        Ref<Texture2D> m_DiffuseTexture, m_SpecularTexture, m_EmissionTexture;
        glm::vec3 m_ObjPositions, m_LightPosition;

        float m_MaterialShininess = 32.0f, m_MaterialEmmisionStength = 1.0f;

        float m_LightAmbient[3] = { 0.2f, 0.2f, 0.2f },
              m_LightDiffuse[3] = { 0.5f, 0.5f, 0.5f },
              m_LightSpecular[3] = { 1.0f, 1.0f, 1.0f };
    };
}