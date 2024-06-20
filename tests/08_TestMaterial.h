#pragma once

#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"

#include "FileSystem/FileSystem.h"

#include "Camera/Camera.h"
#include "Camera/PerspectiveCamera.h"

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
    private:
        Ref<VertexArray> m_Obj_VAO, m_Light_VAO;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;
        Ref<Shader> m_ObjShader, m_LightCubeShader;
        glm::vec3 m_ObjPositions, m_LightPosition;

        float m_MaterialAmbient[3] = { 1.0f, 0.5f, 0.31f },
              m_MaterialDiffuse[3] = { 1.0f, 0.5f, 0.31f },
              m_MaterialSpecular[3] = { 0.5f, 0.5f, 0.5f };
        float m_MaterialShininess = 32.0f;

        float m_LightAmbient[3] = { 0.2f, 0.2f, 0.2f },
              m_LightDiffuse[3] = { 0.5f, 0.5f, 0.5f },
              m_LightSpecular[3] = { 1.0f, 1.0f, 1.0f };
    };
}