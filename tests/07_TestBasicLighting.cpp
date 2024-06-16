#include "07_TestBasicLighting.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include "KeyCodes.h"

namespace test {
    TestBasicLighting::TestBasicLighting(GLFWwindow* window)
      : Test(window), m_ObjPositions(glm::vec3(0.0)), m_LightPosition(glm::vec3(1.2f, 1.0f, 2.0f))
    {
        GLCall(glEnable(GL_DEPTH_TEST));

        // --------------------
        // Camera
        // --------------------
        m_Camera = std::make_unique<Camera>();
        m_Camera->SetPosition({1.2f, 1.2f, 4.2f});

        // --------------------
        // Model datas
        // --------------------
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };
        
        // --------------------
        // Model
        // --------------------
        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        // Object model
        m_Obj_VAO = std::make_unique<VertexArray>();
        m_Obj_VAO->AddBuffer(*m_VBO, layout);

        // Light model
        m_Light_VAO = std::make_unique<VertexArray>();
        m_Light_VAO->AddBuffer(*m_VBO, layout);

        // --------------------
        // Shader
        // --------------------
        // Object shader
        std::string vertexSrc = FileSystem::ReadFile("../res/shaders/Color.vert");
        std::string fragSrc = FileSystem::ReadFile("../res/shaders/Color.frag");
        m_ObjShader = std::make_unique<Shader>(vertexSrc, fragSrc);

        glm::vec3 m_ObjectColor(1.0f, 0.5f, 0.31f);
        glm::vec3 u_LightColor(1.0f, 1.0f, 1.0f);

        // Shader uniforms
        m_ObjShader->Bind();
        m_ObjShader->SetUniform3f("u_ObjectColor", m_ObjectColor.x, m_ObjectColor.y, m_ObjectColor.z);
        m_ObjShader->SetUniformVec3("u_LightColor", u_LightColor);

        // Light shader
        std::string lightVertexSrc = FileSystem::ReadFile("../res/shaders/LightCube.vert");
        std::string lightFragSrc = FileSystem::ReadFile("../res/shaders/LightCube.frag");
        m_LightShader = std::make_unique<Shader>(lightVertexSrc, lightFragSrc);
        m_LightShader->Bind();
        m_LightShader->SetUniformVec3("u_Color", u_LightColor);
    }

    TestBasicLighting::~TestBasicLighting()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    }

    void TestBasicLighting::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestBasicLighting::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        float radius = 2.0f;
        m_LightPosition.x = sin(glfwGetTime()) * radius;
        m_LightPosition.z = cos(glfwGetTime()) * radius;

        {
            Shader::UpdateMatricesView(m_Camera->GetViewMatrix());
            // --------------------
            // Model View Projection
            // --------------------
            glm::mat4 model(1.0f);
            // --------------------
            // Draw object
            // --------------------
            m_ObjShader->Bind();
            m_ObjShader->SetUniformMat4("modelMatrix", model);
            m_ObjShader->SetUniform3f("u_LightPosition", m_LightPosition.x, m_LightPosition.y, m_LightPosition.z);
            m_ObjShader->SetUniform3f("u_CameraPosition", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
            m_ObjShader->SetUniform1f("u_AmbientStrength", m_AmbientStrength);
            m_ObjShader->SetUniform1f("u_SpecularStrength", m_SpecularStrength);
            m_ObjShader->SetUniform1f("u_Shininess", m_Shininess);
            renderer.Draw(*m_ObjShader, *m_Obj_VAO);

            // --------------------
            // Draw light
            // --------------------
            m_LightShader->Bind();
            model = glm::translate(model, m_LightPosition);
            model = glm::scale(model, glm::vec3(0.2f));
            m_LightShader->SetUniformMat4("modelMatrix", model);

            renderer.Draw(*m_LightShader, *m_Light_VAO);
        }
    }

    void TestBasicLighting::OnImGuiRender()
    {
        ImGui::Text("Camera");
        float fov = m_Camera->GetFOV();
        if (ImGui::SliderFloat("FOV", &fov, 0.0f, 180.0f))
            m_Camera->SetFOV(fov);

        glm::vec3 camPos = m_Camera->GetPosition();
        if (ImGui::SliderFloat("CamPosX", &camPos.x, -10.0f, 10.0f))
            m_Camera->SetPositionX(camPos.x);
        if (ImGui::SliderFloat("CamPosY", &camPos.y, -10.0f, 10.0f))
            m_Camera->SetPositionY(camPos.y);
        if (ImGui::SliderFloat("CamPosZ", &camPos.z, -10.0f, 10.0f))
            m_Camera->SetPositionZ(camPos.z);

        ImGui::Text("Light");
        ImGui::SliderFloat("AmbientStrength", &m_AmbientStrength, 0.0f, 1.0f);
        ImGui::SliderFloat("SpecularStrength", &m_SpecularStrength, 0.0f, 10.0f);
        ImGui::SliderInt("Shininess", (int*)&m_Shininess, 0, 256);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}