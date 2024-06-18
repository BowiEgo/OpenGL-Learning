#include "22_TestBlinnPhong.h"

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
    TestBlinnPhong::TestBlinnPhong(GLFWwindow* window)
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
        m_Camera = std::make_shared<Camera>();
        m_Scene->Add(m_Camera);
        // --------------------
        // Light
        // --------------------
        Ref<DirectionalLight> m_DirLight = std::make_shared<DirectionalLight>();
        // m_Scene->Add(m_DirLight);
        
        std::vector<Ref<PointLight>> m_PointLights;
        std::vector<glm::vec3> pointLightPositions = {
            glm::vec3( 0.0f,  1.0f,  0.0f),
        };
        for (unsigned int i = 0; i < pointLightPositions.size(); i++)
        {
            Ref<PointLight> pointLight = std::make_shared<PointLight>(pointLightPositions[i]);
            m_PointLights.push_back(pointLight);
            m_Scene->Add(pointLight);
        }
        // Light cube
        Ref<BoxGeometry> geometry_lightCube = std::make_shared<BoxGeometry>();
        // material
        Ref<BasicMaterial> material_lightCube = std::make_shared<BasicMaterial>();
        material_lightCube->SetColor(glm::vec3(1.0f));
        Ref<Mesh> mesh_lightCube = std::make_shared<Mesh>(geometry_lightCube, material_lightCube);
        mesh_lightCube->SetPosition(m_PointLights[0]->GetPositions());
        mesh_lightCube->SetScale(glm::vec3(0.2f));
        
        m_Scene->Add(mesh_lightCube);

        // --------------------
        // Floor
        // --------------------
        //geometry
        Ref<PlaneGeometry> geometry_container = std::make_shared<PlaneGeometry>();
        // texture
        Ref<Texture2D> diffuseTexture_floor = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/wood.png");
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_floor->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        Ref<StandardMaterial> material_floor = std::make_shared<StandardMaterial>();
        material_floor->AddTexture(diffuseTexture_floor);
        // mesh
        m_Mesh_Floor = std::make_shared<Mesh>(geometry_container, material_floor);

        m_Scene->Add(m_Mesh_Floor);
    }

    TestBlinnPhong::~TestBlinnPhong()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestBlinnPhong::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestBlinnPhong::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        FramebufferManager::s_SMAA_Enabled = m_SMAA_Enabled;

        m_Mesh_Floor->GetMaterial()->UpdateShaderUniform("u_Blinn_Enabled", m_BlinnPhong_Enabled);

        m_Scene->Draw();
    }

    void TestBlinnPhong::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Bullet();ImGui::Text("SMAA");ImGui::SameLine();ImGui::ToggleButton("SMAA", &m_SMAA_Enabled);

        ImGui::Bullet();ImGui::Text("BlinnPhong");ImGui::SameLine();ImGui::ToggleButton("BlinnPhong", &m_BlinnPhong_Enabled);
    }
}
