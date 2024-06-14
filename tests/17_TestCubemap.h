

#pragma once

#include "pch.h"
#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"
#include "Core/TextureCubemap.h"

#include "Camera.h"
#include "Framebuffer.h"
#include "Core/Scene.h"
#include "Core/Model.h"

#include "Core/Light/DirectionalLight.h"
#include "Core/Light/PointLight.h"

namespace test {
    class TestCubemap : public Test
    {
    public:
        TestCubemap(GLFWwindow* window);
        ~TestCubemap();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(float deltaTime);
        void SetCameraAspectRatio(const float aspectRatio) override;
        void EnableCameraControll() override;
        void DisableCameraControll() override;
    private:
        Scene* m_Scene;
        Ref<Camera> m_Camera, m_Camera_Mirror;
        Ref<Framebuffer> m_Framebuffer;
        Ref<DirectionalLight> m_DirectionalLight;
        std::vector<Ref<PointLight>> m_PointLights;

        glm::vec3 m_Mirror_Position = glm::vec3(1.0f, 2.0f, -0.5f);

        Ref<Model> m_Model_Michelle;
        Ref<Mesh> m_Mesh_Floor;
        Ref<InstanceMesh> m_Mesh_Container;
        Ref<InstanceMesh> m_Mesh_Grass;
        Ref<Mesh> m_Mesh_Inversion, m_Mesh_Grayscale, m_Mesh_Sharpen, m_Mesh_Blur;

        CullFaceOption m_CullFaceOption_Floor = CULL_FACE_FRONT;
        CullFaceOption m_CullFaceOption_Container = CULL_FACE_BACK;
        CullFaceOption m_CullFaceOption_Grass = CULL_FACE_NONE;

        bool m_Wireframe_Enabled_Floor = false;
        bool m_Wireframe_Enabled_Container = false;
        bool m_Wireframe_Enabled_Grass = false;

        bool m_Environment_Enabled_Model = true;
        bool m_Environment_Enabled_Container = true;

        float m_Env_Reflection_Rate_Model = 1.0;
        float m_Env_Reflection_Rate_Container = 0.5;

        float m_Env_Refraction_Index_Model = 1.52;
        float m_Env_Refraction_Index_Container = 1.52;
    };
}