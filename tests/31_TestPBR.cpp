#include "31_TestPBR.h"

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

#include "Core/InstanceMesh.h"

namespace test {
    TestPBR::TestPBR(GLFWwindow* window)
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
        m_Camera->SetPosition({ -8.0f, 5.0f, 20.0f });
        m_Scene->Add(m_Camera);
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
        Ref<SphereGeometry> geometry_sphere = std::make_shared<SphereGeometry>(1.0f, 64, 64);
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
                material_sphere->UpdateShaderUniform("u_Albedo", glm::vec3(0.5f, 0.0f, 0.0f));
                material_sphere->UpdateShaderUniform("u_AO", 1.0f);
                // we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                material_sphere->UpdateShaderUniform("u_Roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
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

    TestPBR::~TestPBR()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestPBR::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestPBR::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        m_Scene->Draw();
    }

    void TestPBR::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("CameraPosition %.3f, %.3f, %.3f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
    }
}
