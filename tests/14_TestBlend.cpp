#include "14_TestBlend.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGuiComponents/ToggleButton.h"

#include "Input.h"
#include "KeyCodes.h"

#include "Core/Geometry/GrassGeometry.h"
#include "Core/Geometry/PlaneGeometry.h"
#include "Core/Geometry/BoxGeometry.h"
#include "Core/Material/StandardMaterial.h"

#include "Core/InstanceMesh.h"

namespace test {
    TestBlend::TestBlend(GLFWwindow* window)
      : Test(window)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
            // mesh_container->Is_Transparent = true;
            m_ContainerMeshes.push_back(mesh_container);
            m_Scene->Add(mesh_container);
        }

        // --------------------
        // Vegetation
        // --------------------
        std::vector<glm::vec3> vegetation;
        vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
        vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
        vegetation.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
        vegetation.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
        vegetation.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));

        // texture
        TextureOptions opts = TextureOptions();
        opts.flip = false;
        Ref<Texture2D> diffuseTexture_grass = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/blending_transparent_window.png", opts);
        diffuseTexture_grass->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_grass->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        Ref<StandardMaterial> material_grass = std::make_shared<StandardMaterial>();
        material_grass->SetDiffuseTexture(diffuseTexture_grass);
        // mesh
        Ref<InstanceMesh> mesh_grass = std::make_shared<InstanceMesh>(std::make_shared<GrassGeometry>(), material_grass, vegetation.size());
        for (unsigned int i = 0; i < vegetation.size(); i++)
        {
            mesh_grass->SetPosition(i, vegetation[i]);
        }
        mesh_grass->Is_Transparent = true;
        m_Scene->Add(mesh_grass);
    }

    TestBlend::~TestBlend()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
        delete m_Scene;
    }

    void TestBlend::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestBlend::OnRender()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        Renderer renderer;

        float currentTime = glfwGetTime();

        m_Scene->Draw();
    }

    void TestBlend::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestBlend::ProcessInput(float deltaTime)
    {
        m_Camera->ProcessKeyboardMovement(deltaTime);
        m_Camera->ProcessMouseMovement();
        m_Camera->ProcessMouseScroll();
    }

    void TestBlend::SetCameraAspectRatio(float aspectRatio)
    {
        m_Camera->SetAspectRatio(aspectRatio);
    }

    void TestBlend::EnableCameraControll()
    {
        m_Camera->EnableControll();
    }

    void TestBlend::DisableCameraControll()
    {
        m_Camera->DisableControll();
    }
}
