#include "25_TestNormalMapping.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileSystem/FileSystem.h"
#include "ImGuiComponents/ToggleButton.h"

#include "Input.h"
#include "KeyCodes.h"

#include "Core/Geometry/GrassGeometry.h"
#include "Core/Geometry/QuadGeometry.h"
#include "Core/Geometry/PlaneGeometry.h"
#include "Core/Geometry/BoxGeometry.h"
#include "Core/Geometry/SkyboxGeometry.h"
#include "Core/Material/StandardMaterial.h"
#include "Core/Material/CubemapMaterial.h"

#include "Core/InstanceMesh.h"

namespace test {
    TestNormalMapping::TestNormalMapping(GLFWwindow* window)
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
        m_Camera->SetPosition({ -4.0f, 0.0f, 15.0f });
        m_Scene->Add(m_Camera);
        // --------------------
        // Light
        // --------------------
        m_PointLight = std::make_shared<PointLight>();
        m_PointLight->SetPosition(m_LightPos);
        m_PointLight->SetAmbient( { 1.0f, 1.0f, 1.0f });
        m_Scene->Add(m_PointLight);
        // Light cube
        Ref<BoxGeometry> geometry_lightCube = std::make_shared<BoxGeometry>();
        // material
        Ref<BasicMaterial> material_lightCube = std::make_shared<BasicMaterial>();
        material_lightCube->SetColor(glm::vec3(1.0f));
        m_Mesh_lightCube = std::make_shared<Mesh>(geometry_lightCube, material_lightCube);
        m_Mesh_lightCube->SetPosition(m_LightPos);
        m_Mesh_lightCube->SetScale(glm::vec3(0.2f));
        // m_Scene->Add(m_Mesh_lightCube);
        // --------------------
        // Wall
        // --------------------
        Ref<Texture2D> diffuseTexture_wall = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/brickwall.jpg");
        Ref<Texture2D> normalTexture_wall = std::make_shared<Texture2D>("Texture_Normal", "../res/textures/brickwall_normal.jpg");
        Ref<StandardMaterial> material_wall = std::make_shared<StandardMaterial>();
        material_wall->AddTexture(diffuseTexture_wall);
        material_wall->AddTexture(normalTexture_wall);
        Ref<PlaneGeometry> geometry_wall = std::make_shared<PlaneGeometry>();
        m_Mesh_Wall = std::make_shared<Mesh>(geometry_wall, material_wall);
        m_Scene->Add(m_Mesh_Wall);
        // --------------------
        // Import models
        // --------------------
        m_Model_Nanosuit = std::make_shared<Model>("../res/models/nanosuit/nanosuit.obj");
        m_Model_Nanosuit->Translate(0.0f, -0.5f, -1.0f);
        m_Model_Nanosuit->Scale(0.2f, 0.2f, 0.2f);
        m_Scene->Add(m_Model_Nanosuit);
    }

    TestNormalMapping::~TestNormalMapping()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestNormalMapping::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestNormalMapping::OnRender()
    {
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        // --------------------
        // Draw stage
        // --------------------
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        
        m_Mesh_Wall->GetMaterial()->UpdateShaderUniform("u_NormalMapping_Enabled", m_NormalMapping_Enabled);
        for (auto mesh : m_Model_Nanosuit->GetMeshes())
        {
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->UpdateShaderUniform("u_NormalMapping_Enabled", m_NormalMapping_Enabled);
        }
        m_Mesh_Wall->SetRotation({ glm::radians(m_Rotate_Wall), glm::vec3(1.0f, 0.0f, 0.0f) });
        m_PointLight->SetPosition(m_LightPos);
        m_Mesh_lightCube->SetPosition(m_LightPos);

        m_Scene->Draw();
    }

    void TestNormalMapping::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Bullet();ImGui::Text("NormalMapping");ImGui::SameLine();ImGui::ToggleButton("NormalMapping", &m_NormalMapping_Enabled);
        ImGui::SliderFloat3("Position##PointLight", glm::value_ptr(m_LightPos), -5.0f, 5.0f);
        ImGui::SliderFloat("Rotation##Wall", &m_Rotate_Wall, -90.0f, 90.0f);
    }
}
