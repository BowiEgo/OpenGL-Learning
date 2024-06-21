#pragma once

#include "pch.h"
#include "Test.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"
#include "Core/TextureCubemap.h"

#include "Camera/Camera.h"
#include "Camera/OrthographicCamera.h"
#include "DepthMapFBO.h"
#include "Core/Scene.h"
#include "Core/Model.h"

#include "Core/Light/DirectionalLight.h"
#include "Core/Light/PointLight.h"

#include "Core/Material/BasicMaterial.h"

namespace test {
    class TestShadowMapping : public Test
    {
    public:
        TestShadowMapping(GLFWwindow* window);
        ~TestShadowMapping();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Ref<PerspectiveCamera> m_Viewport_Camera;
        Ref<OrthographicCamera> m_DepthMap_Camera;

        Ref<ShaderMaterial> m_Material_DepthMap;
        Ref<StandardMaterial> m_Material_Wood, m_Material_Container;

        Ref<Mesh> m_Mesh_Floor;
        std::vector<Ref<Mesh>> m_Mesh_Boxes;

        Ref<DepthMapFBO> m_DepthMapFBO;
        Ref<Mesh> m_Mesh_Screen;

        Ref<Texture2D> m_Texture_ShadowMap;
    };
}