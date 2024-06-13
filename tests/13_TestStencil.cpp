#include "13_TestStencil.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGuiComponents/ToggleButton.h"

#include "Input.h"
#include "KeyCodes.h"

#include "Core/Geometry/PlaneGeometry.h"
#include "Core/Geometry/BoxGeometry.h"
#include "Core/Material/StandardMaterial.h"

namespace test {
    TestStencil::TestStencil(GLFWwindow* window)
      : Test(window)
    {
        // --------------------
        // Setup
        // --------------------
        m_Scene = Scene::Create();

        // --------------------
        // Camera
        // --------------------
        m_Camera = std::make_shared<Camera>();
        m_Scene->Add(m_Camera);

        // --------------------
        // Light
        // --------------------
        m_DirectionalLight = std::make_shared<DirectionalLight>();
        m_Scene->Add(m_DirectionalLight);
        
        for (unsigned int i = 0; i < m_PointLightPositions.size(); i++)
        {
            Ref<PointLight> pointLight = std::make_shared<PointLight>(m_PointLightPositions[i]);
            m_PointLights.push_back(pointLight);
            m_Scene->Add(pointLight);
        }

        // --------------------
        // Import models
        // --------------------
        m_Model_Michelle = std::make_shared<Model>("../res/models/michelle/michelle.obj");
        m_Model_Michelle->Translate(0.0f, 0.513f, 0.0f);
        m_Scene->Add(m_Model_Michelle);

        m_Model_Nanosuit = std::make_shared<Model>("../res/models/nanosuit/nanosuit.obj");
        m_Model_Nanosuit->Translate(0.0f, -4.513f, 0.0f);
        m_Model_Nanosuit->Scale(0.2f, 0.2f, 0.2f);
        m_Scene->Add(m_Model_Nanosuit);

        // --------------------
        // Floow
        // --------------------
        // texture
        Ref<Texture2D> diffuseTexture_floor = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/metal.png");
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        Ref<StandardMaterial> material_floor = std::make_shared<StandardMaterial>();
        material_floor->SetDiffuseTexture(diffuseTexture_floor);
        // mesh
        m_Mesh_Floor = std::make_shared<Mesh>(std::make_shared<PlaneGeometry>(), material_floor);
        m_Mesh_Floor->SetPosition(0.0f, -0.01f, 0.0f);
        m_Scene->Add(m_Mesh_Floor);

        // --------------------
        // Container
        // --------------------
        // texture
        Ref<Texture2D> diffuseTexture_container = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/marble.jpg");
        // material
        Ref<StandardMaterial> material_container = std::make_shared<StandardMaterial>();
        material_container->SetDiffuseTexture(diffuseTexture_container);
        // mesh
        m_Mesh_Container = std::make_shared<InstanceMesh>(
            std::make_shared<BoxGeometry>(),
            material_container, m_ContainerPositions.size());

        for (unsigned int i = 0; i < m_ContainerPositions.size(); i++)
        {
            m_Mesh_Container->SetPosition(i, m_ContainerPositions[i]);
            float angle = 20.0f * i;
            std::pair<float, glm::vec3> rotation = { glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f) };
            m_Mesh_Container->SetRotation(i, rotation);
        }
        m_Scene->Add(m_Mesh_Container);
    }

    TestStencil::~TestStencil()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestStencil::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestStencil::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        float currentTime = glfwGetTime();
        
        if (m_Model_Michelle != nullptr)
        {
            float michelleLineWidth = m_Outline_Width * 0.2;
            m_Model_Michelle->Outline_Enabled = m_Model_Outline_Enable;
            m_Model_Michelle->SetOutlineWidth(michelleLineWidth);
            m_Model_Michelle->SetOutlineColor(m_Outline_Color);

            for (auto& mesh : m_Model_Michelle->GetMeshes())
            {
                mesh->GetMaterial()->Wireframe_Enabled = m_Wireframe_Enabled_Model;
            }
        }

        if (m_Model_Nanosuit != nullptr)
        {
            m_Model_Nanosuit->Outline_Enabled = m_Model_Outline_Enable;
            m_Model_Nanosuit->SetOutlineWidth(m_Outline_Width);
            m_Model_Nanosuit->SetOutlineColor(m_Outline_Color);

            for (auto& mesh : m_Model_Nanosuit->GetMeshes())
            {
                mesh->GetMaterial()->Wireframe_Enabled = m_Wireframe_Enabled_Model;
            }
        }

        m_Mesh_Floor->GetMaterial()->Wireframe_Enabled = m_Wireframe_Enabled_Floor;

        m_Mesh_Container->Outline_Enabled = m_Container_Outline_Enable;
        m_Mesh_Container->SetOutlineWidth(m_Outline_Width);
        m_Mesh_Container->SetOutlineColor(m_Outline_Color);
        m_Mesh_Container->GetMaterial()->Wireframe_Enabled = m_Wireframe_Enabled_Container;

        m_Scene->Draw();
    }

    void TestStencil::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::SeparatorText("Outline Parameters");
        ImGui::SliderFloat("Width#Outline", &m_Outline_Width, 0.01f, 20.0f);
        ImGui::ColorEdit4("Color#Outline", glm::value_ptr(m_Outline_Color));

        ImGui::SeparatorText("Objects");

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Floor", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("Wireframe");
            ImGui::SameLine();ImGui::ToggleButton("Wireframe##Floor", &m_Wireframe_Enabled_Floor);
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("Outline");
            ImGui::SameLine();ImGui::ToggleButton("Outline##Model", &m_Model_Outline_Enable);
            ImGui::Text("Wireframe");
            ImGui::SameLine();ImGui::ToggleButton("Wireframe##Model", &m_Wireframe_Enabled_Model);
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Container", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("Outline");
            ImGui::SameLine();ImGui::ToggleButton("Outline##Container", &m_Container_Outline_Enable);
            ImGui::Text("Wireframe");
            ImGui::SameLine();ImGui::ToggleButton("Wireframe##Container", &m_Wireframe_Enabled_Container);
        }
    }

    void TestStencil::ProcessInput(float deltaTime)
    {
        m_Camera->ProcessKeyboardMovement(deltaTime);
        m_Camera->ProcessMouseMovement();
        m_Camera->ProcessMouseScroll();
    }

    void TestStencil::SetCameraAspectRatio(float aspectRatio)
    {
        m_Camera->SetAspectRatio(aspectRatio);
    }

    void TestStencil::EnableCameraControll()
    {
        m_Camera->EnableControll();
    }

    void TestStencil::DisableCameraControll()
    {
        m_Camera->DisableControll();
    }
}
