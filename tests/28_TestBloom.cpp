#include "28_TestBloom.h"

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
    TestBloom::TestBloom(GLFWwindow* window)
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
        m_Camera->SetPosition({ 0.0f, 0.0f, 5.0f });
        m_Scene->Add(m_Camera);
        // --------------------
        // Light
        // --------------------
        // - Positions
        std::vector<glm::vec3> lightPositions;
        lightPositions.push_back(glm::vec3( 0.0f, 0.5f,  1.5f)); // back light
        lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
        lightPositions.push_back(glm::vec3( 3.0f, 0.5f,  1.0f));
        lightPositions.push_back(glm::vec3(-0.8f, 2.4f, -1.0f));
        // - Colors
        std::vector<glm::vec3> lightColors;
        lightColors.push_back(glm::vec3( 5.0f,  5.0f,  5.0f));
        lightColors.push_back(glm::vec3(10.0f,  0.0f,  0.0f));
        lightColors.push_back(glm::vec3( 0.0f,  0.0f, 15.0f));
        lightColors.push_back(glm::vec3( 0.0f,  5.0f,  0.0f));
        
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
            m_Scene->Add(lightCube);
        }
        // --------------------
        // Floor
        // --------------------
        Ref<Texture2D> diffuseTexture_floor = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/wood.png");
        Ref<StandardMaterial> material_floor = std::make_shared<StandardMaterial>();
        material_floor->AddTexture(diffuseTexture_floor);
        Ref<PlaneGeometry> geometry_cube = std::make_shared<PlaneGeometry>();
        m_Mesh_Floor = std::make_shared<Mesh>(geometry_cube, material_floor);
        m_Mesh_Floor->SetPosition({ 0.0f, -0.5f, 0.0f });
        m_Scene->Add(m_Mesh_Floor);
        // --------------------
        // Container
        // --------------------
        // positions
        std::vector<glm::vec3> container_positions = {
            glm::vec3( 0.0f,  1.5f,  0.0f),
            glm::vec3( 2.0f,  0.0f,  1.0f),
            glm::vec3(-1.0f, -1.0f,  2.0f),
            glm::vec3( 0.0f,  2.7f,  4.0f),
            glm::vec3(-2.0f,  1.0f, -3.0f),
            glm::vec3(-3.0f,  0.0f,  0.0f)
        };
        // scales
        std::vector<glm::vec3> container_scales = {
            glm::vec3(0.5f),
            glm::vec3(0.5f),
            glm::vec3(1.0f),
            glm::vec3(1.25f),
            glm::vec3(1.0f),
            glm::vec3(0.5f),
        };
        // rotations
        std::vector<std::pair<float, glm::vec3>> container_rotations = {
            {   0.0f, glm::vec3(1.0, 0.0, 1.0) },
            {   0.0f, glm::vec3(1.0, 0.0, 1.0) },
            {  60.0f, glm::vec3(1.0, 0.0, 1.0) },
            {  23.0f, glm::vec3(1.0, 0.0, 1.0) },
            { 124.0f, glm::vec3(1.0, 0.0, 1.0) },
            {   0.0f, glm::vec3(1.0, 0.0, 1.0) },
        };
        // geometry
        Ref<BoxGeometry> geometry_container = std::make_shared<BoxGeometry>();
        // texture
        Ref<Texture2D> diffuseTexture_container = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/container2.png");
        Ref<Texture2D> specularTexture_container = std::make_shared<Texture2D>("Texture_Specular", "../res/textures/container2_specular.png");
        // material
        Ref<StandardMaterial> material_container = std::make_shared<StandardMaterial>();
        material_container->AddTexture(diffuseTexture_container);
        material_container->AddTexture(specularTexture_container);
        // mesh
        for (unsigned int i = 0; i < container_positions.size(); i++)
        {
            Ref<Mesh> mesh_container = std::make_shared<Mesh>(geometry_container, material_container);
            mesh_container->SetPosition(container_positions[i]);
            mesh_container->SetScale(container_scales[i]);
            mesh_container->SetRotation(container_rotations[i]);

            m_Mesh_Containers.push_back(mesh_container);
            m_Scene->Add(mesh_container);
        }
        // --------------------
        // Preview screen (bright)
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
        // texture
        uint32_t textureID = viewport->GetColorAttachments()[1];
        Ref<Texture2D> texture_preview = std::make_shared<Texture2D>("Texture_Diffuse", textureID);
        // shader
        std::string normalizedVertSrc = FileSystem::ReadFile("../res/shaders/Normalized.vert");
        std::string normalizedFragSrc = FileSystem::ReadFile("../res/shaders/Normalized.frag");
        Ref<Shader> shader_preview = std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc);
        // material
        Ref<ShaderMaterial> material_preview = std::make_shared<ShaderMaterial>(shader_preview);
        material_preview->AddTexture(texture_preview);
        material_preview->UpdateShaderUniform("u_Normalized_Type", -1);
        // mesh
        m_Mesh_Preview = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), material_preview);
        // --------------------
        // Postprocessing
        // --------------------
        // HDR
        m_Postprocessing_Bloom = std::make_shared<PostprocessingBloom>();
    }

    TestBloom::~TestBloom()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestBloom::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestBloom::OnRender()
    {
        // --------------------
        // Draw postprocessing
        // --------------------
        m_Postprocessing_Bloom->Bind();
        // m_Postprocessing_Bloom->GetMaterial()->UpdateShaderUniform("u_Exposure", m_Exposure);
        // m_Postprocessing_Bloom->GetMaterial()->UpdateShaderUniform("u_Split_CoordX", m_ScreenSplit_X);
        m_Scene->Draw();
        m_Postprocessing_Bloom->Render(m_Scene);
        m_Postprocessing_Bloom->Unbind();
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
        
        // m_Scene->Draw();
        // m_Scene->Draw(m_Mesh_Preview.get());
        m_Postprocessing_Bloom->GetScreen()->GetMaterial()->UpdateShaderUniform("u_Bloom_Enabled", m_Bloom_Enabled);
        m_Postprocessing_Bloom->GetScreen()->GetMaterial()->UpdateShaderUniform("u_Exposure", m_Exposure);
        m_Postprocessing_Bloom->GetScreen()->GetMaterial()->UpdateShaderUniform("u_Split_CoordX", m_ScreenSplit_X);
        m_Scene->Draw(m_Postprocessing_Bloom->GetScreen());
        // m_Postprocessing_Bloom->Render(m_Scene);
        viewport->Unbind();
    }

    void TestBloom::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("CameraPosition %.3f, %.3f, %.3f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);

        ImGui::Bullet();ImGui::Text("Bloom");ImGui::SameLine();ImGui::ToggleButton("Bloom", &m_Bloom_Enabled);

        ImGui::SliderFloat("Exposure", &m_Exposure, 0.0f, 5.0f);

        ImGui::Bullet();ImGui::Text("ScreenSplit");
        ImGui::SliderFloat("##", &m_ScreenSplit_X, 0.0f, 1.0f);
    }
}
