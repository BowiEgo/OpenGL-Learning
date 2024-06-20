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
        // m_Camera = std::make_shared<PerspectiveCamera>();
        // m_Scene->Add(m_Camera);
        m_Viewport_Camera = std::make_shared<PerspectiveCamera>();
        m_Viewport_Camera->SetPosition({ -2.0f, 2.0f, -1.0f });
        m_DepthMap_Camera = std::make_shared<OrthographicCamera>();
        m_DepthMap_Camera->SetPosition({ -2.0f, 2.0f, -1.0f });
        m_Camera = m_Viewport_Camera;
        m_Scene->Add(m_Camera);
        m_Scene->Add(m_DepthMap_Camera);
        // m_DepthMap_Camera->DisableControll();
        // m_Scene->Add(m_DepthMap_Camera);
        // --------------------
        // Light
        // --------------------
        Ref<DirectionalLight> m_DirLight = std::make_shared<DirectionalLight>();
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
        // Floor
        // --------------------
        // geometry
        Ref<PlaneGeometry> geometry_floor = std::make_shared<PlaneGeometry>();
        // texture
        Ref<Texture2D> diffuseTexture_floor = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/wood.png");
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        Ref<StandardMaterial> material_floor = std::make_shared<StandardMaterial>();
        material_floor->AddTexture(diffuseTexture_floor);
        // mesh
        m_Mesh_Floor = std::make_shared<Mesh>(geometry_floor, material_floor);

        m_Scene->Add(m_Mesh_Floor);
        // --------------------
        // Container
        // --------------------
        // matrices
        std::vector<glm::vec3> translate_boxes = {
            glm::vec3( 0.0f, 1.5f, 0.0f),
            glm::vec3( 2.0f, 0.0f, 1.0f),
            glm::vec3(-1.0f, 0.0f, 2.0f)
        };
        std::vector<glm::vec3> scale_boxes = {
            glm::vec3( 0.5f),
            glm::vec3( 0.5f),
            glm::vec3(0.25f)
        };
        std::vector<std::pair<float, glm::vec3>> rotate_boxes = {
            std::make_pair( 0.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),  
            std::make_pair( 0.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),  
            std::make_pair(60.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),  
        };
        // geometry
        Ref<BoxGeometry> geometry_box = std::make_shared<BoxGeometry>();
        // material
        Ref<StandardMaterial> material_box = std::make_shared<StandardMaterial>();
        material_box->AddTexture(diffuseTexture_floor);
        // mesh
        for (unsigned int i = 0; i < translate_boxes.size(); i++)
        {
            Ref<Mesh> mesh_box = std::make_shared<Mesh>(geometry_box, material_box);
            mesh_box->SetPosition(translate_boxes[i]);
            mesh_box->SetScale(scale_boxes[i]);
            mesh_box->SetRotation(rotate_boxes[i]);

            m_Mesh_Boxes.push_back(mesh_box);
            m_Scene->Add(mesh_box);
        }

        // --------------------
        // Framebuffer plane
        // --------------------
        FramebufferSpecification fbSpec;
        fbSpec.Width = 1024;
        fbSpec.Height = 1024;
        m_DepthMapFBO = DepthMapFBO::Create(fbSpec);
        // texture
        uint32_t textureID = m_DepthMapFBO->GetDepthMapID();
        Ref<Texture2D> diffuseTexture_mirror = std::make_shared<Texture2D>("Texture_Diffuse", textureID);
        // shader
        std::string normalizedVertSrc = FileSystem::ReadFile("../res/shaders/Normalized.vert");
        std::string normalizedFragSrc = FileSystem::ReadFile("../res/shaders/DepthMap.frag");
        Ref<Shader> screenShader_depthMap = std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc);
        // material
        Ref<ShaderMaterial> material_depthMap = std::make_shared<ShaderMaterial>(screenShader_depthMap);
        material_depthMap->AddTexture(diffuseTexture_mirror);
        // mesh
        m_Mesh_DepthMap = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), material_depthMap);

        // --------------------
        // LightSpaceMatrix
        // --------------------
        GLfloat near_plane = 1.0f, far_plane = 7.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
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
        m_Scene->Draw();
        m_Scene->Draw(m_Mesh_DepthMap.get());
        viewport->Unbind();
    }

    void TestShadowMapping::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
