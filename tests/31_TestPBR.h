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

#include "EnvCubemapFBO.h"

namespace test {
    class TestPBR : public Test
    {
    public:
        TestPBR(GLFWwindow* window);
        ~TestPBR();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::vector<Ref<PointLight>> m_PointLights;
        std::vector<Ref<Mesh>> m_Mesh_LightCubes;

        std::vector<glm::vec3> m_PointLight_Positions = {
            glm::vec3(-10.0f,  10.0f, 10.0f),
            glm::vec3( 10.0f,  10.0f, 10.0f),
            glm::vec3(-10.0f, -10.0f, 10.0f),
            glm::vec3( 10.0f, -10.0f, 10.0f),
        };
        std::vector<glm::vec3> m_PointLight_Colors = {
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f),
            glm::vec3(300.0f, 300.0f, 300.0f)
        };
        
        Ref<EnvCubemapFBO> m_Envmap_FBO;
        glm::vec3 m_Sphere_Color = glm::vec3(0.5f, 0.0f, 0.0f);
        std::vector<Ref<Mesh>> m_Sphere_Meshes;
        Ref<TextureCubemap> m_Irradiance_texture;
        bool m_IrradianceMapEnabled = true;
        bool m_PrefilterMapEnabled = true;

        Ref<Mesh> m_Mesh_BRDF;

        bool m_Map_Enabled;

        std::vector<std::string> m_PBR_Textures = {
            "speckled_countertop",
            "patchy_cement",
            "speckled_rust",
            "armani_marble",
            "gold_scuffed",
            "rustediron",
            "bamboo_wood_semigloss"
        };
    };
}