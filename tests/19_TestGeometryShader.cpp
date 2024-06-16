#include "19_TestGeometryShader.h"

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
    TestGeometryShader::TestGeometryShader(GLFWwindow* window)
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
        // Lights
        Ref<DirectionalLight> dirLight = std::make_shared<DirectionalLight>();
        m_Scene->Add(dirLight);
        
        std::vector<glm::vec3> pointLightPositions = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        };
        for (unsigned int i = 0; i < pointLightPositions.size(); i++)
        {
            Ref<PointLight> pointLight = std::make_shared<PointLight>(pointLightPositions[i]);
            m_Scene->Add(pointLight);
        }
        // --------------------
        // Object
        // --------------------
        // shader
        std::string vertexSrc = FileSystem::ReadFile("../res/shaders/GeometryShader.vert");
        std::string geomSrc = FileSystem::ReadFile("../res/shaders/GeometryShader.geom");
        std::string fragSrc = FileSystem::ReadFile("../res/shaders/GeometryShader.frag");
        Ref<Shader> shader = std::make_shared<Shader>(vertexSrc, geomSrc, fragSrc);
        // geometry
        std::vector<Vertex> m_Vertex = {
            {{  -0.5f,  0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f,  0.0f }, {}, {}, {}, {}},
            {{   0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f,  0.0f }, {}, {}, {}, {}},
            {{   0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f,  1.0f }, {}, {}, {}, {}},
            {{  -0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f,  0.0f }, {}, {}, {}, {}},
        };
        // material
        Ref<ShaderMaterial> material = std::make_shared<ShaderMaterial>(shader);
        // meshes
        Ref<Mesh> mesh = std::make_shared<Mesh>(m_Vertex, material);
        mesh->SetDrawType(DrawType::Points);

        // m_Scene->Add(mesh);

        // --------------------
        // Model
        // --------------------
        // Original model
        m_Model_Nanosuit = std::make_shared<Model>("../res/models/nanosuit/nanosuit.obj");
        m_Model_Nanosuit->Translate(0.0, -2.0, 0.0);
        m_Model_Nanosuit->Scale(0.2, 0.2, 0.2);
        for (auto mesh : m_Model_Nanosuit->GetMeshes())
        {
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->BindGeometryShader("../res/shaders/Explode.geom");
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->Setup();
        }
        m_Scene->Add(m_Model_Nanosuit);
        // Normal visualize
        m_Normal_Visualize_Nanosuit = std::make_shared<Model>("../res/models/nanosuit/nanosuit.obj");
        m_Normal_Visualize_Nanosuit->Translate(0.0, -2.0, 0.0);
        m_Normal_Visualize_Nanosuit->Scale(0.2, 0.2, 0.2);
        for (auto mesh : m_Normal_Visualize_Nanosuit->GetMeshes())
        {
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->BindVertexShader("../res/shaders/NormalVisualize.vert");
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->BindFragmentShader("../res/shaders/NormalVisualize.frag");
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->BindGeometryShader("../res/shaders/NormalVisualize.geom");
            dynamic_cast<StandardMaterial*>(mesh->GetMaterial().get())->Setup();
        }
        m_Scene->Add(m_Normal_Visualize_Nanosuit);
    }

    TestGeometryShader::~TestGeometryShader()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestGeometryShader::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestGeometryShader::OnRender()
    {
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        // float currentTime = glfwGetTime();
        for (auto mesh : m_Model_Nanosuit->GetMeshes())
        {
            // mesh->GetMaterial()->UpdateShaderUniform("u_Time", currentTime);
            mesh->GetMaterial()->UpdateShaderUniform("u_ExplodeProgress", m_ExplodeProgress);
        }

        for (auto mesh : m_Normal_Visualize_Nanosuit->GetMeshes())
        {
            mesh->GetMaterial()->UpdateShaderUniform("u_Split_CoordX", test::GetViewportSize().x * m_ScreenSplit_X);
        }
        m_Scene->Draw();
    }

    void TestGeometryShader::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Bullet();ImGui::Text("ScreenSplit");
        ImGui::SliderFloat("##", &m_ScreenSplit_X, 0.0f, 1.0f);

        ImGui::Bullet();ImGui::Text("Explode");
        ImGui::SliderFloat("progress##Nanosuit", &m_ExplodeProgress, 0.0f, 1.0f);
    }
}
