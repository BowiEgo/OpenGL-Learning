#include "32_TestDiffuseIrradiance.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileSystem/FileSystem.h"
#include "ImGuiComponents/ToggleButton.h"

#include "Input.h"
#include "KeyCodes.h"

#include "Core/Geometry/BoxGeometry.h"
#include "Core/Geometry/SphereGeometry.h"
#include "Core/Material/StandardMaterial.h"
#include "Core/Material/CubemapMaterial.h"

#include "Core/InstanceMesh.h"

namespace test {
    TestDiffuseIrradiance::TestDiffuseIrradiance(GLFWwindow* window)
      : Test(window)
    {
        // --------------------
        // Setup
        // --------------------
        m_Scene = Scene::Create();
        // --------------------
        // Camera
        // --------------------
        m_Camera = std::make_shared<PerspectiveCamera>();
        m_Camera->SetPosition({ 0.0f, 0.0f, 24.0f });
        m_Scene->Add(m_Camera);
        // --------------------
        // Environment
        // --------------------
        m_Envmap_FBO = EnvCubemapFBO::Create(); // must be a private member for now.
        m_Envmap_FBO->RenderCubemap(m_Scene);
        m_Envmap_FBO->RenderIrradianceMap(m_Scene);
        // skybox
        Ref<TextureCubemap> skybox_texture = std::make_shared<TextureCubemap>("Texture_Environment", m_Envmap_FBO->GetEnvmapID());
        Ref<CubemapMaterial> skybox_material = std::make_shared<CubemapMaterial>();
        skybox_material->SetCubemapTexture(skybox_texture);
        Ref<Mesh> skybox_mesh = std::make_shared<Mesh>(std::make_shared<BoxGeometry>(), skybox_material);
        m_Scene->SetSkybox(skybox_mesh);
        // irradianceMap
        // --------------------
        // Light
        // --------------------
        // PointLight
        for (int i = 0; i < m_PointLight_Positions.size(); i++)
        {
            Ref<PointLight> light = std::make_shared<PointLight>();
            light->SetPosition(m_PointLight_Positions[i]);
            light->SetColor(m_PointLight_Colors[i]);
            m_PointLights.push_back(light);
            m_Scene->Add(light);

            // Light cube
            Ref<BoxGeometry> geometry_lightCube = std::make_shared<BoxGeometry>();
            // material
            Ref<BasicMaterial> material_lightCube = std::make_shared<BasicMaterial>();
            material_lightCube->SetColor(m_PointLight_Colors[i]);
            Ref<Mesh> lightCube = std::make_shared<Mesh>(geometry_lightCube, material_lightCube);
            lightCube->SetPosition(m_PointLight_Positions[i]);
            lightCube->SetScale(glm::vec3(0.2f));
            m_Mesh_LightCubes.push_back(lightCube);

            // m_Scene->Add(lightCube);
        }
        // --------------------
        // Spheres
        // --------------------
        Ref<TextureCubemap> irradiance_texture = std::make_shared<TextureCubemap>("Texture_Environment", m_Envmap_FBO->GetIrradianceMapID());
        // geometry
        Ref<SphereGeometry> geometry_sphere = std::make_shared<SphereGeometry>(1.0f, 64, 64);
        // shader
        std::string vertSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.vert");
        std::string fragSrc = FileSystem::ReadFile("../res/shaders/PBR.frag");

        int nrRows    = 7;
        int nrColumns = 7;
        float spacing = 2.5;
        for (int row = 0; row < nrRows; ++row)
        {
            for (int col = 0; col < nrColumns; ++col) 
            {
                Ref<Shader> shader_sphere = std::make_shared<Shader>(vertSrc, fragSrc);
                Ref<ShaderMaterial> material_sphere = std::make_shared<ShaderMaterial>(shader_sphere);
                material_sphere->UpdateShaderUniform("u_Metallic", (float)row / (float)nrRows);
                material_sphere->UpdateShaderUniform("u_Albedo",   glm::vec3(0.5f, 0.0f, 0.0f));
                material_sphere->UpdateShaderUniform("u_AO",       1.0f);
                // we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                material_sphere->UpdateShaderUniform("u_Roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
                material_sphere->AddTexture("irradianceMap", irradiance_texture);

                Ref<Mesh> mesh_sphere = std::make_shared<Mesh>(geometry_sphere, material_sphere);

                mesh_sphere->SetPosition(glm::vec3(
                    (col - (nrColumns / 2)) * spacing,
                    (row - (nrRows / 2)) * spacing,
                    0.0f
                ));

                m_Sphere_Meshes.push_back(mesh_sphere);
                m_Scene->Add(mesh_sphere);
            }
        }
    }

    TestDiffuseIrradiance::~TestDiffuseIrradiance()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestDiffuseIrradiance::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestDiffuseIrradiance::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        OnViewPortResize();

        for (auto mesh : m_Sphere_Meshes)
        {
            mesh->GetMaterial()->UpdateShaderUniform("u_IrradianceMapEnabled", m_IrradianceMapEnabled);
        }

        m_Scene->Draw();
    }

    void TestDiffuseIrradiance::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("CameraPosition %.3f, %.3f, %.3f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);

        ImGui::Bullet();ImGui::Text("IrradianceMapping");ImGui::SameLine();ImGui::ToggleButton("IrradianceMapping", &m_IrradianceMapEnabled);
    }
}
