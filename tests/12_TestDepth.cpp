#include "12_TestDepth.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGuiComponents/ToggleButton.h"
#include "FileSystem/FileSystem.h"

#include "Input.h"
#include "KeyCodes.h"

#include "Core/Geometry/PlaneGeometry.h"
#include "Core/Geometry/BoxGeometry.h"
#include "Core/Material/StandardMaterial.h"
#include "Core/Material/ShaderMaterial.h"

namespace test {
    TestDepth::TestDepth(GLFWwindow* window)
      : Test(window)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
        // GLCall(glDepthFunc(GL_ALWAYS));
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
        // Depth
        // --------------------
        // shader
        std::string depthVertSrc = FileSystem::ReadFile("../res/shaders/Depth.vert");
        std::string depthFragSrc = FileSystem::ReadFile("../res/shaders/Depth.frag");
        m_DepthShader = std::make_shared<Shader>(depthVertSrc, depthFragSrc);
        // material
        Ref<ShaderMaterial> material_depth = std::make_shared<ShaderMaterial>(m_DepthShader);

        // --------------------
        // Import models
        // --------------------
        m_ModelMichelle = std::make_shared<Model>("../res/models/michelle/michelle.obj");
        m_ModelMichelle->Translate(0.0f, 0.513f, 0.0f);
        std::vector<Ref<Mesh>> modelMeshes = m_ModelMichelle->GetMeshes();
        for (unsigned int i = 0; i < modelMeshes.size(); i++)
        {
            modelMeshes[i]->SetMaterial(material_depth);
        }
        m_Scene->Add(m_ModelMichelle);

        // --------------------
        // Plane
        // --------------------
        // texture
        Ref<Texture2D> diffuseTexture_plane = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/metal.png");
        // diffuseTexture_plane->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        // diffuseTexture_plane->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        // Ref<StandardMaterial> material_plane = std::make_shared<StandardMaterial>();
        // material_plane->SetDiffuseTexture(diffuseTexture_plane);
        // mesh
        Ref<Mesh> mesh_plane = std::make_shared<Mesh>(std::make_shared<PlaneGeometry>(), material_depth);
        m_Scene->Add(mesh_plane);

        // --------------------
        // Container
        // --------------------
        // texture
        // Ref<Texture2D> diffuseTexture_container = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/marble.jpg");
        // material
        // Ref<ShaderMaterial> material_container = std::make_shared<ShaderMaterial>(m_DepthShader);
        // material_container->SetDiffuseTexture(diffuseTexture_container);
        // mesh
        for (unsigned int i = 0; i < m_ObjPositions.size(); i++)
        {
            Ref<Mesh> mesh_container = std::make_shared<Mesh>(std::make_shared<BoxGeometry>(), material_depth);
            mesh_container->SetPosition(m_ObjPositions[i]);
            float angle = 20.0f * i;
            std::pair<float, glm::vec3> rotation = { glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f) };
            mesh_container->SetRotation(rotation);

            m_Scene->Add(mesh_container);
        }
    }

    TestDepth::~TestDepth()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestDepth::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestDepth::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        float currentTime = glfwGetTime();

        for (unsigned int i = 0; i < m_Scene->GetPointLights().size(); i++)
        {
            m_Scene->GetPointLights()[i]->SetEnabled(m_IsPointLightsEnabled);
        }

        m_DepthShader->Bind();
        m_DepthShader->SetUniform1f("u_Near", m_ZBuffer_Near);
        m_DepthShader->SetUniform1f("u_Far", m_ZBuffer_Far);

        m_Scene->Draw();
    }

    void TestDepth::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::SliderFloat("ZBuffer_Near##ZBuffer", &m_ZBuffer_Near, 0.1f, 1.0f);
        ImGui::SliderFloat("ZBuffer_Far##ZBuffer", &m_ZBuffer_Far, 0.1f, 100.0f);
    }

    void TestDepth::ProcessInput(float deltaTime)
    {
        m_Camera->ProcessKeyboardMovement(deltaTime);
        m_Camera->ProcessMouseMovement();
        m_Camera->ProcessMouseScroll();
    }

    void TestDepth::EnableCameraControll()
    {
        m_Camera->EnableControll();
    }

    void TestDepth::DisableCameraControll()
    {
        m_Camera->DisableControll();
    }
}
