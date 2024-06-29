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

#include "Postprocessing.h"
#include "PostprocessingBloom.h"

namespace test {
    class TestBloom : public Test
    {
    public:
        TestBloom(GLFWwindow* window);
        ~TestBloom();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::vector<Ref<PointLight>> m_PointLights;
        std::vector<Ref<Mesh>> m_Mesh_LightCubes;
        std::vector<Ref<Mesh>> m_Mesh_Containers;
        Ref<Mesh> m_Mesh_Floor;

        Ref<Mesh> m_Mesh_Preview;

        Ref<PostprocessingBloom> m_Postprocessing_Bloom;
        float m_Exposure = 0.5f;
        float m_ScreenSplit_X = 0.5f;

        bool m_Bloom_Enabled = true;
    };
}