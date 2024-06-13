#pragma once

#include "pch.h"
#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"

#include "Camera.h"
#include "Core/Scene.h"
#include "Core/Model.h"

#include "Core/Light/DirectionalLight.h"
#include "Core/Light/PointLight.h"

namespace test {
    class TestStencil : public Test
    {
    public:
        TestStencil(GLFWwindow* window);
        ~TestStencil();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(float deltaTime);
        void SetCameraAspectRatio(const float aspectRatio) override;
        void EnableCameraControll() override;
        void DisableCameraControll() override;
    private:
        Scene* m_Scene;

        Ref<Camera> m_Camera;
        Ref<DirectionalLight> m_DirectionalLight;
        std::vector<Ref<PointLight>> m_PointLights;

        std::vector<glm::vec3> m_ContainerPositions = {
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
        std::vector<glm::vec3> m_PointLightPositions = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        };

        Ref<Mesh> m_Mesh_Floor;
        Ref<Model> m_Model_Michelle;
        Ref<Model> m_Model_Nanosuit;
        Ref<InstanceMesh> m_Mesh_Container;

        bool m_Model_Outline_Enable = true, m_Container_Outline_Enable = true;
        float m_Outline_Width = 0.5f;

        bool m_Wireframe_Enabled_Floor = false;
        bool m_Wireframe_Enabled_Model = false;
        bool m_Wireframe_Enabled_Container = false;

        glm::vec3 m_Outline_Color = glm::vec3(0.7647f, 0.9294f, 0.9137f);
    };
}