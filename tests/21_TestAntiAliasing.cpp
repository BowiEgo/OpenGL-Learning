#include "21_TestAntiAliasing.h"

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
    TestAntiAliasing::TestAntiAliasing(GLFWwindow* window)
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
        m_Camera = std::make_shared<PerspectiveCamera>();
        m_Scene->Add(m_Camera);
        // --------------------
        // Light
        // --------------------
        Ref<DirectionalLight> m_DirLight = std::make_shared<DirectionalLight>();
        m_Scene->Add(m_DirLight);
        
        std::vector<Ref<PointLight>> m_PointLights;
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
        // Object
        // --------------------
        //geometry
        Ref<BoxGeometry> geometry_container = std::make_shared<BoxGeometry>();
        // material
        m_Container_Material = std::make_shared<BasicMaterial>();
        // mesh
        Ref<Mesh> mesh_container = std::make_shared<Mesh>(geometry_container, m_Container_Material);

        m_Scene->Add(mesh_container);
    }

    TestAntiAliasing::~TestAntiAliasing()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestAntiAliasing::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestAntiAliasing::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        FramebufferManager::s_SMAA_Enabled = m_SMAA_Enabled;

        m_Container_Material->SetColor(m_Container_Color);

        m_Scene->Draw();
    }

    void TestAntiAliasing::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Bullet();ImGui::Text("SMAA");ImGui::SameLine();ImGui::ToggleButton("SMAA", &m_SMAA_Enabled);

        ImGui::Bullet();ImGui::Text("Box");
        ImGui::ColorEdit4("Color##Container", glm::value_ptr(m_Container_Color));
    }
}
