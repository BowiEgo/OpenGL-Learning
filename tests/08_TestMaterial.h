#pragma once

#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"

#include "FileSystem/FileSystem.h"

#include "Camera.h"

#include <memory>

namespace test {
    class TestMaterial : public Test
    {
    public:
        TestMaterial(GLFWwindow* window);
        ~TestMaterial();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(float deltaTime);
        void SetCameraAspectRatio(const float aspectRatio) override;
        void EnableCameraControll() override;
        void DisableCameraControll() override;
    private:
        std::unique_ptr<VertexArray> m_Obj_VAO, m_Light_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_ObjShader, m_LightCubeShader;
        glm::vec3 m_ObjPositions, m_LightPosition;

        std::unique_ptr<Camera> m_Camera;

        float m_MaterialAmbient[3] = { 1.0f, 0.5f, 0.31f },
              m_MaterialDiffuse[3] = { 1.0f, 0.5f, 0.31f },
              m_MaterialSpecular[3] = { 0.5f, 0.5f, 0.5f };
        float m_MaterialShininess = 32.0f;

        float m_LightAmbient[3] = { 0.2f, 0.2f, 0.2f },
              m_LightDiffuse[3] = { 0.5f, 0.5f, 0.5f },
              m_LightSpecular[3] = { 1.0f, 1.0f, 1.0f };
    };
}