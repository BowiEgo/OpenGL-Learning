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
        m_ModelMichelle = std::make_shared<Model>("../res/models/michelle/michelle.obj");
        m_ModelMichelle->Translate(0.0f, 0.513f, 0.0f);
        std::vector<Ref<Mesh>> modelMeshes = m_ModelMichelle->GetMeshes();
        m_Scene->Add(m_ModelMichelle);

        m_ModelNanosuit = std::make_shared<Model>("../res/models/nanosuit/nanosuit.obj");
        m_ModelNanosuit->Translate(0.0f, -4.513f, 0.0f);
        m_ModelNanosuit->Scale(0.2f, 0.2f, 0.2f);
        m_Scene->Add(m_ModelNanosuit);

        // --------------------
        // Plane
        // --------------------
        // texture
        Ref<Texture2D> diffuseTexture_plane = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/metal.png");
        diffuseTexture_plane->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_plane->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        Ref<StandardMaterial> material_plane = std::make_shared<StandardMaterial>();
        material_plane->SetDiffuseTexture(diffuseTexture_plane);
        // mesh
        Ref<Mesh> mesh_plane = std::make_shared<Mesh>(std::make_shared<PlaneGeometry>(), material_plane);
        mesh_plane->SetPosition(0.0f, -0.01f, 0.0f);
        m_Scene->Add(mesh_plane);

        // --------------------
        // Container
        // --------------------
        // texture
        Ref<Texture2D> diffuseTexture_container = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/marble.jpg");
        // material
        Ref<StandardMaterial> material_container = std::make_shared<StandardMaterial>();
        material_container->SetDiffuseTexture(diffuseTexture_container);
        // mesh
        for (unsigned int i = 0; i < m_ObjPositions.size(); i++)
        {
            Ref<Mesh> mesh_container = std::make_shared<Mesh>(std::make_shared<BoxGeometry>(), material_container);
            mesh_container->SetPosition(m_ObjPositions[i]);
            float angle = 20.0f * i;
            std::pair<float, glm::vec3> rotation = { glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f) };
            mesh_container->SetRotation(rotation);
            m_ContainerMeshes.push_back(mesh_container);
            m_Scene->Add(mesh_container);
        }
    }

    TestStencil::~TestStencil()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
        delete m_Scene;
    }

    void TestStencil::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestStencil::OnRender()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        Renderer renderer;

        float currentTime = glfwGetTime();

        float michelleLineWidth = m_Outline_Width * 0.2;
        m_ModelMichelle->Outline_Enabled = m_Model_Outline_Enable;
        m_ModelMichelle->SetOutlineWidth(michelleLineWidth);

        m_ModelNanosuit->Outline_Enabled = m_Model_Outline_Enable;
        m_ModelNanosuit->SetOutlineWidth(m_Outline_Width);

        for (unsigned int i = 0; i < m_ContainerMeshes.size(); i++)
        {
            m_ContainerMeshes[i]->Outline_Enabled = m_Container_Outline_Enable;
            m_ContainerMeshes[i]->SetOutlineWidth(m_Outline_Width);
        }

        m_Scene->Update();
    }

    void TestStencil::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Bullet();ImGui::Text("Outline");
        ImGui::SliderFloat("Width#Outline", &m_Outline_Width, 0.01f, 20.0f);

        ImGui::Bullet();ImGui::Text("Model");
        ImGui::SameLine();ImGui::ToggleButton("Outline##Model", &m_Model_Outline_Enable);

        ImGui::Bullet();ImGui::Text("Container");
        ImGui::SameLine();ImGui::ToggleButton("Outline##Container", &m_Container_Outline_Enable);
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
