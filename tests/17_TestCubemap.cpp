#include "17_TestCubemap.h"

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
#include "Core/Material/BasicMaterial.h"
#include "Core/Material/CubemapMaterial.h"

#include "Core/InstanceMesh.h"

namespace test {
    TestCubemap::TestCubemap(GLFWwindow* window)
      : Test(window)
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        // --------------------
        // Setup
        // --------------------
        m_Scene = Scene::Create();

        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        // --------------------
        // Camera
        // --------------------
        m_Camera = std::make_shared<Camera>();
        m_Camera_Mirror = std::make_shared<Camera>();
        // m_Camera_Mirror->SetPosition(m_Mirror_Position);
        // m_Camera_Mirror->DisableControll();
        m_Scene->Add(m_Camera);
        m_Scene->Add(m_Camera_Mirror);

        // --------------------
        // Light
        // --------------------
        m_DirectionalLight = std::make_shared<DirectionalLight>();
        m_Scene->Add(m_DirectionalLight);
        
        std::vector<glm::vec3> pointLightPositions = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        };
        for (unsigned int i = 0; i < pointLightPositions.size(); i++)
        {
            Ref<PointLight> pointLight = std::make_shared<PointLight>(pointLightPositions[i]);
            m_PointLights.push_back(pointLight);
            m_Scene->Add(pointLight);
        }

        // --------------------
        // Skybox
        // --------------------
        std::vector<std::string> skybox_filePaths = {
            "../res/textures/cubemaps/skybox/px.jpg",
            "../res/textures/cubemaps/skybox/nx.jpg",
            "../res/textures/cubemaps/skybox/py.jpg",
            "../res/textures/cubemaps/skybox/ny.jpg",
            "../res/textures/cubemaps/skybox/pz.jpg",
            "../res/textures/cubemaps/skybox/nz.jpg"
        };
        Ref<TextureCubemap> skybox_texture = std::make_shared<TextureCubemap>("Texture_Environment", skybox_filePaths);
        // geometry
        Ref<BoxGeometry> skybox_geometry = std::make_shared<BoxGeometry>();
        // material
        Ref<CubemapMaterial> skybox_material = std::make_shared<CubemapMaterial>();
        skybox_material->SetCubemapTexture(skybox_texture);
        // mesh
        Ref<Mesh> skybox_mesh = std::make_shared<Mesh>(skybox_geometry, skybox_material);
        m_Scene->SetSkybox(skybox_mesh);

        // --------------------
        // Import models
        // --------------------
        m_Model_Michelle = std::make_shared<Model>("../res/models/michelle/michelle.obj");
        m_Model_Michelle->Translate(0.0f, 0.513f, 0.0f);
        m_Scene->Add(m_Model_Michelle);

        m_Model_Nanosuit = std::make_shared<Model>("../res/models/nanosuit/nanosuit.obj");
        m_Model_Nanosuit->Scale(0.16f, 0.16f, 0.16f);
        m_Model_Nanosuit->Translate(2.0f, 0.513f, 0.0f);
        m_Scene->Add(m_Model_Nanosuit);

        // --------------------
        // Floor
        // --------------------
        // texture
        Ref<Texture2D> diffuseTexture_floor = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/metal.png");
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        Ref<StandardMaterial> material_floor = std::make_shared<StandardMaterial>();
        material_floor->AddTexture(diffuseTexture_floor);
        // mesh
        m_Mesh_Floor = std::make_shared<Mesh>(std::make_shared<PlaneGeometry>(), material_floor);
        m_Mesh_Floor->SetPosition(0.0f, -0.01f, 0.0f);
        m_Mesh_Floor->Cull_Face = m_CullFaceOption_Floor;
        m_Scene->Add(m_Mesh_Floor);

        // --------------------
        // Container
        // --------------------
        std::vector<glm::vec3> containerPositions = {
            glm::vec3( 0.0f,  0.0f,   0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f,  -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f,  -3.5f),
            glm::vec3(-1.7f,  3.0f,  -7.5f),
            glm::vec3( 1.3f, -2.0f,  -2.5f),
            glm::vec3( 1.5f,  2.0f,  -2.5f),
            glm::vec3( 1.5f,  0.2f,  -1.5f),
            glm::vec3(-1.3f,  1.0f,  -1.5f)  
        };
        // texture
        Ref<Texture2D> diffuseTexture_container = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/container.jpg");
        // material
        Ref<StandardMaterial> material_container = std::make_shared<StandardMaterial>();
        material_container->AddTexture(diffuseTexture_container);
        // mesh
        m_Mesh_Container = std::make_shared<InstanceMesh>(
            std::make_shared<BoxGeometry>(),
            material_container, containerPositions.size());

        for (unsigned int i = 0; i < containerPositions.size(); i++)
        {
            m_Mesh_Container->SetPosition(i, containerPositions[i]);
            float angle = 20.0f * i;
            std::pair<float, glm::vec3> rotation = { glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f) };
            m_Mesh_Container->SetRotation(i, rotation);
        }
        m_Mesh_Container->Cull_Face = m_CullFaceOption_Container;
        m_Scene->Add(m_Mesh_Container);

        // --------------------
        // Vegetation
        // --------------------
        std::vector<glm::vec3> vegetation;
        vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.48f));
        vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.51f));
        vegetation.push_back(glm::vec3( 1.2f,  0.0f,   0.7f));
        vegetation.push_back(glm::vec3(-1.5f,  0.0f,   1.1f));
        vegetation.push_back(glm::vec3( 1.1f,  0.0f,   2.6f));
        // texture
        TextureOptions grassTexOpts = TextureOptions();
        grassTexOpts.flip = false;
        Ref<Texture2D> diffuseTexture_grass = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/grass.png", grassTexOpts);
        diffuseTexture_grass->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_grass->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        Ref<StandardMaterial> material_grass = std::make_shared<StandardMaterial>();
        material_grass->AddTexture(diffuseTexture_grass);
        material_grass->Discard_Transparent = true;
        // mesh
        m_Mesh_Grass = std::make_shared<InstanceMesh>(
            std::make_shared<GrassGeometry>(),
            material_grass, vegetation.size());

        for (unsigned int i = 0; i < vegetation.size(); i++)
        {
            m_Mesh_Grass->SetPosition(i, vegetation[i]);
        }

        m_Mesh_Grass->Cull_Face = m_CullFaceOption_Grass;
        m_Scene->Add(m_Mesh_Grass);

        // --------------------
        // Framebuffer plane
        // --------------------
        // texture
        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        Ref<Texture2D> diffuseTexture_mirror = std::make_shared<Texture2D>("Texture_Diffuse", textureID);
        // shader
        std::string normalizedVertSrc = FileSystem::ReadFile("../res/shaders/Normalized.vert");
        std::string normalizedFragSrc = FileSystem::ReadFile("../res/shaders/Normalized.frag");
        Ref<Shader> screenShader_inversion = std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc, "../res/shader/Normalized");
        Ref<Shader> screenShader_grayscale = std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc, "../res/shader/Normalized");
        Ref<Shader> screenShader_sharpe = std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc, "../res/shader/Normalized");
        Ref<Shader> screenShader_blur = std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc, "../res/shader/Normalized");
        // material
        Ref<ShaderMaterial> material_inversion = std::make_shared<ShaderMaterial>(screenShader_inversion);
        Ref<ShaderMaterial> material_grayscale = std::make_shared<ShaderMaterial>(screenShader_grayscale);
        Ref<ShaderMaterial> material_sharpen = std::make_shared<ShaderMaterial>(screenShader_sharpe);
        Ref<ShaderMaterial> material_blur = std::make_shared<ShaderMaterial>(screenShader_blur);
        material_inversion->AddTexture(diffuseTexture_mirror);
        material_grayscale->AddTexture(diffuseTexture_mirror);
        material_sharpen->AddTexture(diffuseTexture_mirror);
        material_blur->AddTexture(diffuseTexture_mirror);
        // mesh
        m_Mesh_Inversion = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), material_inversion);
        m_Mesh_Inversion->GetMaterial()->UpdateShaderUniform("u_Normalized_Type", 0);
        m_Mesh_Grayscale = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), material_grayscale);
        m_Mesh_Grayscale->GetMaterial()->UpdateShaderUniform("u_Normalized_Type", 1);
        m_Mesh_Sharpen = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), material_sharpen);
        m_Mesh_Sharpen->GetMaterial()->UpdateShaderUniform("u_Normalized_Type", 2);
        m_Mesh_Blur = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), material_blur);
        m_Mesh_Blur->GetMaterial()->UpdateShaderUniform("u_Normalized_Type", 3);
        // m_Mesh_Mirror->SetPosition(m_Mirror_Position);
        // m_Mesh_Mirror->GetMaterial()->Wireframe_Enabled = true;
    }

    TestCubemap::~TestCubemap()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestCubemap::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestCubemap::OnRender()
    {
        float currentTime = glfwGetTime();
        // --------------------
        // Draw postprocessing
        // --------------------
        m_Framebuffer->Bind();
        // m_Camera_Mirror->SetTarget(m_Camera->GetPosition());
        m_Scene->SetCurrentCamera(m_Camera);
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glEnable(GL_DEPTH_TEST));

        m_Scene->Draw();
        m_Framebuffer->Unbind();

        // --------------------
        // Draw stage
        // --------------------
        FramebufferManager::GetByTag("viewport")->Bind();
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        m_Scene->SetCurrentCamera(m_Camera);

        for (auto mesh : m_Model_Michelle->GetMeshes())
        {
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->Environment_Enabled = m_Environment_Enabled_Model;
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->SetEnvironmentReflectiveRate(m_Env_Reflective_Rate_Model);
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->SetEnvironmentRefractiveRate(m_Env_Refractive_Rate_Model);
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->SetEnvironmentRefractiveIndex(m_Env_Refractive_Index_Model);
        }

        for (auto mesh : m_Model_Nanosuit->GetMeshes())
        {
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->Environment_Enabled = m_Environment_Enabled_Model;
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->SetEnvironmentReflectiveRate(m_Env_Reflective_Rate_Model);
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->SetEnvironmentRefractiveRate(m_Env_Refractive_Rate_Model);
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->SetEnvironmentRefractiveIndex(m_Env_Refractive_Index_Model);
        }

        dynamic_cast<StandardMaterial*>(m_Mesh_Container->GetMaterial().get())->Environment_Enabled = m_Environment_Enabled_Container;
        dynamic_cast<StandardMaterial*>(m_Mesh_Container->GetMaterial().get())->SetEnvironmentReflectiveRate(m_Env_Reflective_Rate_Container);
        dynamic_cast<StandardMaterial*>(m_Mesh_Container->GetMaterial().get())->SetEnvironmentRefractiveRate(m_Env_Refractive_Rate_Container);
        dynamic_cast<StandardMaterial*>(m_Mesh_Container->GetMaterial().get())->SetEnvironmentRefractiveIndex(m_Env_Refractive_Index_Container);

        m_Scene->Draw();
        m_Scene->Draw(m_Mesh_Inversion.get());
        m_Scene->Draw(m_Mesh_Grayscale.get());
        m_Scene->Draw(m_Mesh_Sharpen.get());
        m_Scene->Draw(m_Mesh_Blur.get());
        FramebufferManager::GetByTag("viewport")->Unbind();
    }

    void TestCubemap::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("EnvironmentMapping");ImGui::SameLine();
            ImGui::ToggleButton("EnvironmentMapping##Model", &m_Environment_Enabled_Model);
            ImGui::SliderFloat("EnvReflectiveRate##Model", &m_Env_Reflective_Rate_Model, 0.0f, 1.0f);
            ImGui::SliderFloat("EnvRefractiveRate##Model", &m_Env_Refractive_Rate_Model, 0.0f, 1.0f);
            ImGui::SliderFloat("EnvRefractiveIndex##Model", &m_Env_Refractive_Index_Model, 0.0f, 5.0f);
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Container", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("EnvironmentMapping");ImGui::SameLine();
            ImGui::ToggleButton("EnvironmentMapping##Container", &m_Environment_Enabled_Container);
            ImGui::SliderFloat("EnvReflectiveRate##Container", &m_Env_Reflective_Rate_Container, 0.0f, 1.0f);
            ImGui::SliderFloat("EnvRefractiveRate##Container", &m_Env_Refractive_Rate_Container, 0.0f, 1.0f);
            ImGui::SliderFloat("EnvRefractiveIndex##Container", &m_Env_Refractive_Index_Container, 0.0f, 5.0f);
        }
    }

    void TestCubemap::ProcessInput(float deltaTime)
    {
        m_Camera->ProcessKeyboardMovement(deltaTime);
        m_Camera->ProcessMouseMovement();
        m_Camera->ProcessMouseScroll();

        m_Camera_Mirror->ProcessKeyboardMovement(deltaTime);
        m_Camera_Mirror->ProcessMouseMovement();
        m_Camera_Mirror->ProcessMouseScroll();
    }

    void TestCubemap::EnableCameraControll()
    {
        m_Camera->EnableControll();
    }

    void TestCubemap::DisableCameraControll()
    {
        m_Camera->DisableControll();
    }
}
