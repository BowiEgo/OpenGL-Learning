#include "23_TestShadowMapping.h"

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
    TestShadowMapping::TestShadowMapping(GLFWwindow* window)
      : Test(window)
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        // --------------------
        // Setup
        // --------------------
        m_Scene = Scene::Create();
        // --------------------
        // Camera
        // --------------------
        glm::vec3 dirLight_Dir = glm::vec3(2.0f, -2.0f, 1.0f);
        m_Viewport_Camera = std::make_shared<PerspectiveCamera>();
        m_Viewport_Camera->SetPosition({ -2.0f, 2.0f, -1.0f });
        m_DepthMap_Camera = std::make_shared<OrthographicCamera>();
        m_DepthMap_Camera->SetPosition(-dirLight_Dir);
        m_Camera = m_Viewport_Camera;
        m_Scene->Add(m_Camera);
        m_Scene->Add(m_DepthMap_Camera);
        // --------------------
        // Light
        // --------------------
        Ref<DirectionalLight> m_DirLight = std::make_shared<DirectionalLight>();
        m_DirLight->SetDirection(dirLight_Dir);
        m_Scene->Add(m_DirLight);
        
        // std::vector<Ref<PointLight>> m_PointLights;
        // std::vector<glm::vec3> pointLightPositions = {
        //     glm::vec3( -2.0f, 4.0f, -1.0f ),
        // };
        // for (unsigned int i = 0; i < pointLightPositions.size(); i++)
        // {
        //     Ref<PointLight> pointLight = std::make_shared<PointLight>(pointLightPositions[i]);
        //     m_PointLights.push_back(pointLight);
        //     m_Scene->Add(pointLight);
        // }
        // // Light cube
        // Ref<BoxGeometry> geometry_lightCube = std::make_shared<BoxGeometry>();
        // // material
        // Ref<BasicMaterial> material_lightCube = std::make_shared<BasicMaterial>();
        // material_lightCube->SetColor(glm::vec3(1.0f));
        // Ref<Mesh> mesh_lightCube = std::make_shared<Mesh>(geometry_lightCube, material_lightCube);
        // mesh_lightCube->SetPosition(m_PointLights[0]->GetPositions());
        // mesh_lightCube->SetScale(glm::vec3(0.2f));
        
        // m_Scene->Add(mesh_lightCube);

        // --------------------
        // Framebuffer plane
        // --------------------
        FramebufferSpecification fbSpec;
        fbSpec.Width = 1024;
        fbSpec.Height = 1024;
        m_DepthMapFBO = DepthMapFBO::Create(fbSpec);
        // texture
        Ref<Texture2D> diffuseTexture_screen = std::make_shared<Texture2D>("Texture_Diffuse", m_DepthMapFBO->GetDepthMapID());
        // shader
        std::string screenVertSrc = FileSystem::ReadFile("../res/shaders/DepthMap_Screen.vert");
        std::string screenFragSrc = FileSystem::ReadFile("../res/shaders/DepthMap_Screen.frag");
        Ref<Shader> shader_screen = std::make_shared<Shader>(screenVertSrc, screenFragSrc);
        // material
        Ref<ShaderMaterial> material_screen = std::make_shared<ShaderMaterial>(shader_screen);
        material_screen->AddTexture(diffuseTexture_screen);
        // mesh
        m_Mesh_Screen = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), material_screen);

        // --------------------
        // ShadowMap texture
        // --------------------
        // shader
        std::string depthMapVertSrc = FileSystem::ReadFile("../res/shaders/DepthMap.vert");
        std::string depthMapFragSrc = FileSystem::ReadFile("../res/shaders/DepthMap.frag");
        Ref<Shader> shader_depthMap = std::make_shared<Shader>(depthMapVertSrc, depthMapFragSrc);
        // material
        m_Material_DepthMap = std::make_shared<ShaderMaterial>(shader_depthMap);
        // texture
        m_Texture_ShadowMap = std::make_shared<Texture2D>("Texture_ShadowMap", m_DepthMapFBO->GetDepthMapID());

        // --------------------
        // Floor
        // --------------------
        // geometry
        Ref<PlaneGeometry> geometry_floor = std::make_shared<PlaneGeometry>();
        // texture
        Ref<Texture2D> diffuseTexture_floor = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/wood.png");
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        m_Material_Wood = std::make_shared<StandardMaterial>();
        m_Material_Wood->AddTexture(diffuseTexture_floor);
        m_Material_Wood->AddTexture(m_Texture_ShadowMap);
        // mesh
        m_Mesh_Floor = std::make_shared<Mesh>(geometry_floor, m_Material_Wood);

        // m_Scene->Add(m_Mesh_Floor); // exclude floor when drawing to resolve shadow acne.
        // --------------------
        // Container
        // --------------------
        // matrices
        std::vector<glm::vec3> translate_boxes = {
            glm::vec3( 0.0f, 1.5f, 0.0f),
            glm::vec3( 2.0f, 0.0f, 1.0f),
            glm::vec3(-1.0f, 0.0f, 2.0f),
            glm::vec3( 1.0f, 0.0f, -2.0f)
        };
        std::vector<glm::vec3> scale_boxes = {
            glm::vec3( 0.5f),
            glm::vec3( 0.5f),
            glm::vec3(0.25f),
            glm::vec3( 1.0f),
        };
        std::vector<std::pair<float, glm::vec3>> rotate_boxes = {
            std::make_pair( 0.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),  
            std::make_pair( 0.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),  
            std::make_pair(60.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
            std::make_pair(0.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
        };
        // geometry
        Ref<BoxGeometry> geometry_box = std::make_shared<BoxGeometry>();
        // texture
        Ref<Texture2D> diffuseTexture_container = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/container2.png");
        Ref<Texture2D> specularTexture_container = std::make_shared<Texture2D>("Texture_Specular", "../res/textures/container2_specular.png");
        // material
        m_Material_Container = std::make_shared<StandardMaterial>();
        m_Material_Container->AddTexture(diffuseTexture_container);
        m_Material_Container->AddTexture(specularTexture_container);
        m_Material_Container->AddTexture(m_Texture_ShadowMap);
        // mesh
        for (unsigned int i = 0; i < translate_boxes.size(); i++)
        {
            Ref<Mesh> mesh_box = std::make_shared<Mesh>(geometry_box, m_Material_Container);
            mesh_box->SetPosition(translate_boxes[i]);
            mesh_box->SetScale(scale_boxes[i]);
            mesh_box->SetRotation(rotate_boxes[i]);

            m_Mesh_Boxes.push_back(mesh_box);
            m_Scene->Add(mesh_box);
        }
        // --------------------
        // Import models
        // --------------------
        m_Model_Michelle = std::make_shared<Model>("../res/models/michelle/michelle.obj");
        m_Model_Michelle->Translate(-1.0f, -0.5f, 0.0f);
        std::pair rotate_Michelle = std::make_pair(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        m_Model_Michelle->Rotate(rotate_Michelle);
        for (auto mesh : m_Model_Michelle->GetMeshes())
        {
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->AddTexture(m_Texture_ShadowMap);
        }
        m_Scene->Add(m_Model_Michelle);
    }

    TestShadowMapping::~TestShadowMapping()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestShadowMapping::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestShadowMapping::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        // --------------------
        // Draw postprocessing
        // --------------------
        m_DepthMapFBO->Bind();
        GLCall(glEnable(GL_DEPTH_TEST));
        m_Scene->SetCurrentCamera(m_DepthMap_Camera);
        m_Mesh_Floor->SetMaterial(m_Material_DepthMap);
        for (auto mesh : m_Mesh_Boxes)
        {
            mesh->SetMaterial(m_Material_DepthMap);
            mesh->Cull_Face = CULL_FACE_FRONT;

        }
        for (auto mesh : m_Model_Michelle->GetMeshes())
        {
            mesh->Cull_Face = CULL_FACE_FRONT;
        }
        m_Scene->Draw();
        m_DepthMapFBO->Unbind();

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
        m_Scene->SetCurrentCamera(m_Viewport_Camera);
        OnViewPortResize();
        m_Material_Wood->UpdateShaderUniform("u_Shadow_Enabled", m_Shadow_Enabled);
        m_Material_Wood->UpdateShaderUniform("u_LightSpaceMatrix", m_DepthMap_Camera->GetProjMatrix() * m_DepthMap_Camera->GetViewMatrix()); // add lightSpaceMatrix to shader
        m_Mesh_Floor->SetMaterial(m_Material_Wood);
        for (auto mesh : m_Mesh_Boxes)
        {
            m_Material_Container->UpdateShaderUniform("u_Shadow_Enabled", m_Shadow_Enabled);
            m_Material_Container->UpdateShaderUniform("u_LightSpaceMatrix", m_DepthMap_Camera->GetProjMatrix() * m_DepthMap_Camera->GetViewMatrix()); // add lightSpaceMatrix to shader
            mesh->SetMaterial(m_Material_Container);
            mesh->Cull_Face = CULL_FACE_BACK;
        }
        for (auto mesh : m_Model_Michelle->GetMeshes())
        {
            m_Material_Container->UpdateShaderUniform("u_Shadow_Enabled", m_Shadow_Enabled);
            m_Material_Container->UpdateShaderUniform("u_LightSpaceMatrix", m_DepthMap_Camera->GetProjMatrix() * m_DepthMap_Camera->GetViewMatrix()); // add lightSpaceMatrix to shader
            mesh->Cull_Face = CULL_FACE_BACK;
        }
        m_Scene->Draw();
        m_Scene->Draw(m_Mesh_Floor.get());
        m_Scene->Draw(m_Mesh_Screen.get());
        viewport->Unbind();
    }

    void TestShadowMapping::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Bullet();ImGui::Text("Shadow");ImGui::SameLine();ImGui::ToggleButton("DirectionalShadow", &m_Shadow_Enabled);
    }
}
