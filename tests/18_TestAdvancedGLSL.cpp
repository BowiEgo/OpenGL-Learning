#include "18_TestAdvancedGLSL.h"

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
    TestAdvancedGLSL::TestAdvancedGLSL(GLFWwindow* window)
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
        // Container
        // --------------------
        std::vector<glm::vec3> containerPositions = {
            glm::vec3(-1.0f,  1.0f, 0.0f),
            glm::vec3( 1.0f,  1.0f, 0.0f),
            glm::vec3(-1.0f, -1.0f, 0.0f),
            glm::vec3( 1.0f, -1.0f, 0.0f),
        };
        std::vector<glm::vec3> containerColors = {
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 0.0f),
        };
        // shader
        std::string vertexSrc = FileSystem::ReadFile("../res/shaders/UniformBuffer.vert");
        std::string fragSrc = FileSystem::ReadFile("../res/shaders/UniformBuffer.frag");
        // geometry
        Ref<BoxGeometry> geometry_box = std::make_shared<BoxGeometry>();
        // meshes
        for (unsigned int i = 0; i < containerPositions.size(); i++)
        {
            Ref<Shader> shader_container = std::make_shared<Shader>(vertexSrc, fragSrc);
            Ref<ShaderMaterial> material_container = std::make_shared<ShaderMaterial>(shader_container);
            material_container->UpdateShaderUniform("u_Color", containerColors[i]);

            Ref<Mesh> mesh = std::make_shared<Mesh>(geometry_box, material_container);
            mesh->SetPosition(containerPositions[i]);

            m_Mesh_Container.push_back(mesh);
            m_Scene->Add(mesh);
        }
    }

    TestAdvancedGLSL::~TestAdvancedGLSL()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestAdvancedGLSL::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestAdvancedGLSL::OnRender()
    {
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        for (int i = 0; i < m_Mesh_Container.size(); i++)
        {
            m_Mesh_Container[i]->GetMaterial()->UpdateShaderUniform("u_Split_CoordX", test::GetViewportSize().x * m_ScreenSplit_X);
        }

        m_Scene->Draw();
    }

    void TestAdvancedGLSL::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Bullet();ImGui::Text("ScreenSplit");
        ImGui::SliderFloat("##", &m_ScreenSplit_X, 0.0f, 1.0f);
    }
}
