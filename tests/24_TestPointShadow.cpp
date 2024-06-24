#include "24_TestPointShadow.h"

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
    TestPointShadow::TestPointShadow(GLFWwindow* window)
      : Test(window)
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        unsigned int SHADOW_WIDTH = 1024;
        unsigned int SHADOW_HEIGHT = 1024;
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
        m_DepthMap_Camera = std::make_shared<PerspectiveCamera>(m_LightPos, glm::vec3(0.0f), 90.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, 1.0f, 25.0f);
        m_Camera = m_Viewport_Camera;
        m_Scene->Add(m_Camera);
        m_Scene->Add(m_DepthMap_Camera);
        // --------------------
        // Light
        // --------------------
        m_PointLight = std::make_shared<PointLight>();
        m_PointLight->SetPosition(m_LightPos);
        m_PointLight->SetAmbient({ 1.0f, 1.0f, 1.0f });
        m_Scene->Add(m_PointLight);
        // Light cube
        Ref<BoxGeometry> geometry_lightCube = std::make_shared<BoxGeometry>();
        // material
        Ref<BasicMaterial> material_lightCube = std::make_shared<BasicMaterial>();
        material_lightCube->SetColor(glm::vec3(1.0f));
        Ref<Mesh> mesh_lightCube = std::make_shared<Mesh>(geometry_lightCube, material_lightCube);
        mesh_lightCube->SetPosition(m_LightPos);
        mesh_lightCube->SetScale(glm::vec3(0.2f));
        
        m_Scene->Add(mesh_lightCube);

        // --------------------
        // ShadowMap
        // --------------------
        
        // shader
        std::string depthMapVertSrc = FileSystem::ReadFile("../res/shaders/DepthMap_PointShadow.vert");
        std::string depthMapGeomSrc = FileSystem::ReadFile("../res/shaders/DepthMap_PointShadow.geom");
        std::string depthMapFragSrc = FileSystem::ReadFile("../res/shaders/DepthMap_PointShadow.frag");
        Ref<Shader> shader_depthMap = std::make_shared<Shader>(depthMapVertSrc, depthMapGeomSrc, depthMapFragSrc);
        // material
        GLfloat aspect = (GLfloat)SHADOW_WIDTH/(GLfloat)SHADOW_HEIGHT;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, m_Near, m_Far);
        m_ShadowTransforms.push_back(shadowProj * 
                        glm::lookAt(m_LightPos, m_LightPos + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
        m_ShadowTransforms.push_back(shadowProj * 
                        glm::lookAt(m_LightPos, m_LightPos + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
        m_ShadowTransforms.push_back(shadowProj * 
                        glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0)));
        m_ShadowTransforms.push_back(shadowProj * 
                        glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0)));
        m_ShadowTransforms.push_back(shadowProj * 
                        glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,-1.0,0.0)));
        m_ShadowTransforms.push_back(shadowProj * 
                        glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,-1.0,0.0)));
        m_Material_DepthMap = std::make_shared<ShaderMaterial>(shader_depthMap);
        m_Material_DepthMap->GetShader()->Bind();
        m_Material_DepthMap->UpdateShaderUniform("u_ShadowMatrices", m_ShadowTransforms);
        m_Material_DepthMap->UpdateShaderUniform("u_Far", m_Far);
        m_Material_DepthMap->UpdateShaderUniform("u_LightPos", m_LightPos);

        // --------------------
        // Framebuffer
        // --------------------
        FramebufferSpecification fbSpec;
        fbSpec.Width = (uint32_t)SHADOW_WIDTH;
        fbSpec.Height = (uint32_t)SHADOW_HEIGHT;
        m_DepthMapFBO = DepthMapFBO::Create("TextureCubemap", fbSpec);
        // shader
        std::string screenVertSrc = FileSystem::ReadFile("../res/shaders/DepthMap_Screen.vert");
        std::string screenFragSrc = FileSystem::ReadFile("../res/shaders/DepthMap_Screen.frag");
        Ref<Shader> shader_screen = std::make_shared<Shader>(screenVertSrc, screenFragSrc);
        // texture
        Ref<TextureCubemap> diffuseTexture_screen = std::make_shared<TextureCubemap>("Texture_Diffuse", m_DepthMapFBO->GetColorAttachmentRendererID());
        // material
        Ref<ShaderMaterial> material_screen = std::make_shared<ShaderMaterial>(shader_screen);
        // material_screen->AddTexture("Texture_Diffuse", diffuseTexture_screen);
        // mesh
        // m_Mesh_Screen = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), material_screen);

        m_Texture_ShadowMap = std::make_shared<TextureCubemap>("Texture_CubeShadowMap", m_DepthMapFBO->GetDepthMapID());
        // --------------------
        // Room
        // --------------------
        // geometry
        Ref<BoxGeometry> geometry_room = std::make_shared<BoxGeometry>();
        // texture
        Ref<Texture2D> diffuseTexture_floor = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/wood.png");
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        m_Material_Wood = std::make_shared<StandardMaterial>();
        m_Material_Wood->AddTexture(diffuseTexture_floor);
        m_Material_Wood->AddTexture(m_Texture_ShadowMap);
        // mesh
        m_Mesh_Room = std::make_shared<Mesh>(geometry_room, m_Material_Wood);
        m_Mesh_Room->SetScale(glm::vec3(10.0f));

        // m_Scene->Add(m_Mesh_Room);

        // --------------------
        // Container
        // --------------------
        // matrices
        std::vector<glm::vec3> translate_boxes = {
            glm::vec3( 4.0f, -3.5f, 0.0f),
            glm::vec3( 2.0f,  3.0f, 1.0f),
            glm::vec3(-3.0f, -1.0f, 0.0f),
            glm::vec3(-1.5f,  1.0f, 1.5f),
            glm::vec3(-1.5f,  2.0f,-3.0f)
        };
        std::vector<glm::vec3> scale_boxes = {
            glm::vec3(1.0f),
            glm::vec3(1.5f),
            glm::vec3(1.0f),
            glm::vec3(1.0f),
            glm::vec3(1.5f)
        };
        std::vector<std::pair<float, glm::vec3>> rotate_boxes = {
            std::make_pair( 0.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
            std::make_pair( 0.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
            std::make_pair( 0.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
            std::make_pair( 0.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
            std::make_pair(60.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))),
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
    }

    TestPointShadow::~TestPointShadow()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestPointShadow::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestPointShadow::OnRender()
    {
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        // GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        // --------------------
        // Draw postprocessing
        // --------------------
        m_DepthMapFBO->Bind();
        GLCall(glEnable(GL_DEPTH_TEST));
        m_Mesh_Room->SetMaterial(m_Material_DepthMap);
        m_Mesh_Room->Cull_Face = CULL_FACE_FRONT;
        for (auto mesh : m_Mesh_Boxes)
        {
            mesh->SetMaterial(m_Material_DepthMap);
            mesh->Cull_Face = CULL_FACE_FRONT;
        }
        m_Scene->Draw(m_Mesh_Room.get());
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
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        m_Scene->SetCurrentCamera(m_Viewport_Camera);
        OnViewPortResize();
        m_Material_Wood->UpdateShaderUniform("u_Shadow_Enabled", m_Shadow_Enabled);
        m_Material_Wood->UpdateShaderUniform("u_Far", m_Far);
        m_Mesh_Room->SetMaterial(m_Material_Wood);
        for (auto mesh : m_Mesh_Boxes)
        {
            m_Material_Container->UpdateShaderUniform("u_Shadow_Enabled", m_Shadow_Enabled);
            m_Material_Container->UpdateShaderUniform("u_Far", m_Far);
            mesh->SetMaterial(m_Material_Container);
            mesh->Cull_Face = CULL_FACE_BACK;
        }
        m_Scene->Draw();
        m_Scene->Draw(m_Mesh_Room.get());
        viewport->Unbind();
    }

    void TestPointShadow::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Bullet();ImGui::Text("Shadow");ImGui::SameLine();ImGui::ToggleButton("PointShadow", &m_Shadow_Enabled);
    }
}
