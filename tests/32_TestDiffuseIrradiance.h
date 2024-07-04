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
    class TestDiffuseIrradiance : public Test
    {
    public:
        TestDiffuseIrradiance(GLFWwindow* window);
        ~TestDiffuseIrradiance();

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
        std::vector<Ref<Mesh>> m_Sphere_Meshes;
        Ref<TextureCubemap> m_Irradiance_texture;
        bool m_IrradianceMapEnabled = true;
    };
}