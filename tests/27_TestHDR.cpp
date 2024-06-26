#include "27_TestHDR.h"

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
    TestHDR::TestHDR(GLFWwindow* window)
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
        m_Camera->SetPosition({ 0.0f, -0.25f, 30.0f });
        m_Scene->Add(m_Camera);
        // --------------------
        // Light
        // --------------------
        // - Positions
        std::vector<glm::vec3> lightPositions;
        lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
        lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
        lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
        lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
        // - Colors
        std::vector<glm::vec3> lightColors;
        lightColors.push_back(glm::vec3(100.0f, 100.0f, 100.0f));
        lightColors.push_back(glm::vec3(0.5f, 0.0f, 0.0f));
        lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.8f));
        lightColors.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
        
        // PointLight
        for (int i = 0; i < lightPositions.size(); i++)
        {
            Ref<PointLight> light = std::make_shared<PointLight>();
            light->SetPosition(lightPositions[i]);
            light->SetColor(lightColors[i]);
            m_PointLights.push_back(light);

            // Light cube
            Ref<BoxGeometry> geometry_lightCube = std::make_shared<BoxGeometry>();
            // material
            Ref<BasicMaterial> material_lightCube = std::make_shared<BasicMaterial>();
            material_lightCube->SetColor(lightColors[i]);
            Ref<Mesh> lightCube = std::make_shared<Mesh>(geometry_lightCube, material_lightCube);
            lightCube->SetPosition(lightPositions[i]);
            lightCube->SetScale(glm::vec3(0.2f));
            m_Mesh_LightCubes.push_back(lightCube);

            m_Scene->Add(light);
            // m_Scene->Add(lightCube);
        }
        // --------------------
        // Cube
        // --------------------
        Ref<Texture2D> diffuseTexture_cube = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/wood.png");
        Ref<StandardMaterial> material_cube = std::make_shared<StandardMaterial>();
        material_cube->AddTexture(diffuseTexture_cube);
        Ref<BoxGeometry> geometry_cube = std::make_shared<BoxGeometry>();
        m_Mesh_Cube = std::make_shared<Mesh>(geometry_cube, material_cube);
        m_Mesh_Cube->SetPosition({ 0.0f, 0.0f, 25.0f });
        m_Mesh_Cube->SetScale({ 5.0f, 5.0f, 55.0f });
        m_Scene->Add(m_Mesh_Cube);
        // --------------------
        // Postprocessing
        // --------------------
        // HDR
        m_Postprocessing_HDR = std::make_shared<Postprocessing>();
    }

    TestHDR::~TestHDR()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestHDR::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestHDR::OnRender()
    {
        // --------------------
        // Draw postprocessing
        // --------------------
        m_Postprocessing_HDR->Bind();
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        m_Postprocessing_HDR->GetMaterial()->UpdateShaderUniform("u_Exposure", m_Exposure);
        m_Postprocessing_HDR->GetMaterial()->UpdateShaderUniform("u_Split_CoordX", m_ScreenSplit_X);
        m_Scene->Draw();
        m_Postprocessing_HDR->Unbind();
        // --------------------
        // Draw stage
        // --------------------
        Framebuffer* viewport;
        if (FramebufferManager::s_SMAA_Enabled)
        {
            viewport = FramebufferManager::GetByTag("viewport").get();
        }
        else
        {
            viewport = FramebufferManager::GetByTag("nonMultisample_viewport").get();
        }
        viewport->Bind();
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        OnViewPortResize();
        
        m_Scene->Draw(m_Postprocessing_HDR->GetScreen());
        viewport->Unbind();
    }

    void TestHDR::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("CameraPosition %.3f, %.3f, %.3f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);

        ImGui::SliderFloat("Exposure", &m_Exposure, 0.0f, 5.0f);

        ImGui::Bullet();ImGui::Text("ScreenSplit");
        ImGui::SliderFloat("##", &m_ScreenSplit_X, 0.0f, 1.0f);
    }
}
