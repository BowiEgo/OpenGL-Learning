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
    class TestFaceCulling : public Test
    {
    public:
        TestFaceCulling(GLFWwindow* window);
        ~TestFaceCulling();

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

        Ref<Mesh> m_Mesh_Floor;
        Ref<InstanceMesh> m_Mesh_Container;
        Ref<InstanceMesh> m_Mesh_Grass;

        CullFaceOption m_CullFaceOption_Floor = CULL_FACE_FRONT;
        CullFaceOption m_CullFaceOption_Container = CULL_FACE_BACK;
        CullFaceOption m_CullFaceOption_Grass = CULL_FACE_NONE;

        bool m_Wireframe_Enabled_Floor = false;
        bool m_Wireframe_Enabled_Container = false;
        bool m_Wireframe_Enabled_Grass = false;
    };
}