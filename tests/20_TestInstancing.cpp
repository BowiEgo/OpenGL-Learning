#include "20_TestInstancing.h"

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
    TestInstancing::TestInstancing(GLFWwindow* window)
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
        m_Camera->SetFar(500.0f);
        m_Camera->SetPositionZ(150.0f);
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
        // shader
        std::string vertexSrc = R"(
            #version 330 core
            layout (location = 0) in vec2 a_Position;
            layout (location = 1) in vec3 a_Color;
            layout (location = 7) in vec2 a_Offset;

            out vec3 f_Color;

            void main()
            {
                vec2 pos = a_Position * (gl_InstanceID / 100.0);
                gl_Position = vec4(pos + a_Offset, 0.0, 1.0);
                f_Color = a_Color;
            }
        )";
        std::string fragSrc = R"(
            #version 330 core
            out vec4 FragColor;

            in vec3 f_Color;

            void main()
            {
                FragColor = vec4(f_Color, 1.0);
            }
        )";
        m_Shader = std::make_shared<Shader>(vertexSrc, fragSrc);
        // geometry
        std::vector<Vertex> vertexs = {
            {{  -0.05f,  0.05f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f,  0.0f }, {}, {}, {}, {}},
            {{   0.05f, -0.05f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f,  0.0f }, {}, {}, {}, {}},
            {{  -0.05f, -0.05f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f,  1.0f }, {}, {}, {}, {}},
            {{  -0.05f,  0.05f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f,  0.0f }, {}, {}, {}, {}},
            {{   0.05f, -0.05f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f,  0.0f }, {}, {}, {}, {}},
            {{   0.05f,  0.05f, 0.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f,  0.0f }, {}, {}, {}, {}},
        };
        // material
        Ref<ShaderMaterial> material = std::make_shared<ShaderMaterial>(m_Shader);
        // meshes
        Ref<Mesh> mesh = std::make_shared<Mesh>(vertexs, material);

        // instance VBO
        unsigned int size = 100;
        glm::vec2 translations[size];
        int index = 0;
        float offset = 0.1f;
        for(int y = -10; y < 10; y += 2)
        {
            for(int x = -10; x < 10; x += 2)
            {
                glm::vec2 translation;
                translation.x = (float)x / 10.0f + offset;
                translation.y = (float)y / 10.0f + offset;
                translations[index++] = translation;
            }
        }
        Ref<VertexBuffer> instanceVBO = std::make_shared<VertexBuffer>(&translations[0], sizeof(glm::vec2) * 100);

        mesh->SetDrawWay(DrawWay::Instanced, size);
        mesh->AddInstanceVBO(instanceVBO, 2, sizeof(float));
        
        // m_Scene->Add(mesh);

        // --------------------
        // Models
        // --------------------
        // planet
        Ref<Model> Model_Planet = std::make_shared<Model>("../res/models/planet/planet.obj");
        Model_Planet->Translate(0.0f, -3.0f, 0.0f);
        Model_Planet->Scale(4.0f, 4.0f, 4.0f);
        m_Scene->Add(Model_Planet);
        // rock
        m_Model_Rock = std::make_shared<Model>("../res/models/rock/rock.obj");
        // matrices
        srand(static_cast<unsigned int>(glfwGetTime()));
        float radius = 150.0;
        float offset_rock = 25.0f;

        for(unsigned int i = 0; i < m_Amount; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);

            float angle = (float)i / (float)m_Amount * 360.0f;
            float displacement = (rand() % (int)(2 * offset_rock * 100)) / 100.0f - offset_rock;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset_rock * 100)) / 100.0f - offset_rock;
            float y = displacement * 0.4f;
            displacement = (rand() % (int)(2 * offset_rock * 100)) / 100.0f - offset_rock;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));
            m_Translates.push_back(glm::vec3(x, y, z));

            float scale = (rand() % 20) / 100.0f;
            model = glm::scale(model, glm::vec3(scale));
            m_Scales.push_back(glm::vec3(scale));

            float rotAngle = (rand() % 360);
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
            m_Rotates.push_back(std::make_pair(rotAngle, glm::vec3(0.4f, 0.6f, 0.8f)));

            m_ModelMatrices[i] = model;
        }
        // instance VBO
        m_InstanceVBO_Rock = std::make_shared<VertexBuffer>(&m_ModelMatrices[0], sizeof(glm::mat4) * m_Amount);
        std::vector<Ref<Mesh>> meshes_Rock = m_Model_Rock->GetMeshes();
        GLsizei vec4Size = sizeof(glm::vec4);
        for (unsigned int i = 0; i < meshes_Rock.size(); i++)
        {
            meshes_Rock[i]->AddInstanceVBO(m_InstanceVBO_Rock, 4, vec4Size, 4);
        }
    }

    TestInstancing::~TestInstancing()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestInstancing::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestInstancing::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        if (m_Instancing_Enabled)
        {
            std::vector<Ref<Mesh>> meshes_Rock = m_Model_Rock->GetMeshes();
            GLsizei vec4Size = sizeof(glm::vec4);
            for (unsigned int i = 0; i < meshes_Rock.size(); i++)
            {
                meshes_Rock[i]->SetDrawWay(DrawWay::Instanced, m_Amount);
                meshes_Rock[i]->GetMaterial()->UpdateShaderUniform("u_Is_Instance", true);
            }
            m_Scene->Draw(m_Model_Rock.get());
        }
        else
        {
            std::vector<Ref<Mesh>> meshes_Rock = m_Model_Rock->GetMeshes();
            for (unsigned int i = 0; i < meshes_Rock.size(); i++)
            {
                meshes_Rock[i]->SetDrawWay(DrawWay::None, m_Amount);
                meshes_Rock[i]->GetMaterial()->UpdateShaderUniform("u_Is_Instance", false);
            }
            for (unsigned int i = 0; i < m_Translates.size(); i++)
            {
                m_Model_Rock->Translate(m_Translates[i]);
                m_Model_Rock->Scale(m_Scales[i]);
                m_Model_Rock->Rotate(m_Rotates[i]);
                m_Scene->Draw(m_Model_Rock.get());
            }
        }

        m_Scene->Draw();
    }

    void TestInstancing::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Bullet();ImGui::Text("Instancing");ImGui::SameLine();ImGui::ToggleButton("Instancing", &m_Instancing_Enabled);
    }
}
