#pragma once

#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"

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
        void SetCameraAspectRatio(const float aspectRatio) override;
        void EnableCameraControll() override;
        void DisableCameraControll() override;
    private:
        Ref<VertexArray> m_Obj_VAO, m_Light_VAO;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;
        Ref<Shader> m_ObjShader, m_LightShader;
        glm::vec3 m_ObjPositions, m_LightPosition;

        glm::mat4 m_Proj, m_View;
        float m_Mix = 0.2;

        Ref<Camera> m_Camera;

        float m_AmbientStrength = 0.1f, m_SpecularStrength = 0.5f;
        unsigned int m_Shininess = 32;
    };
}