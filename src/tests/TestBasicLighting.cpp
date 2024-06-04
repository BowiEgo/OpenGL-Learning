#include "TestBasicLighting.h"

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
        m_Camera->SetPositionZ(6.0f);
        m_Camera->SetPosition({1.2f, 1.2f, 4.2f});
        // glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // --------------------
        // Model datas
        // --------------------
        GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // A 0
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // B 1
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // C 2
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // D 3
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // E 4
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   // F 5
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // G 6
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   // H 7
    
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // D 8
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // A 9
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  // E 10
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // H 11
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // B 12
             0.5f,  0.5f, -0.5f,  1.0f, 0.0f,   // C 13
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   // G 14
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f,   // F 15
    
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // A 16
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // B 17
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f,   // F 18
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  // E 19
             0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  // C 20
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  // D 21
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // H 22
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // G 23
        };
        // index data
        GLuint indices[] = {
            // front and back
             0,  3,  2,
             2,  1,  0,
             4,  5,  6,
             6,  7,  4,
            // left and right
            11,  8,  9,
             9, 10, 11,
            12, 13, 14,
            14, 15, 12,
            // bottom and top
            16, 17, 18,
            18, 19, 16,
            20, 21, 22,
            22, 23, 20
        };
        
        // --------------------
        // Model
        // --------------------
        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);

        m_IBO = std::make_unique<IndexBuffer>(indices, 36);

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
        m_ObjShader->SetUniform3f("u_LightColor", u_LightColor.x, u_LightColor.y, u_LightColor.z);

        // Light shader
        std::string lightVertexSrc = FileSystem::ReadFile("../res/shaders/LightCube.vert");
        std::string lightFragSrc = FileSystem::ReadFile("../res/shaders/LightCube.frag");
        m_LightShader = std::make_unique<Shader>(lightVertexSrc, lightFragSrc);
    }

    TestBasicLighting::~TestBasicLighting()
    {
    }

    void TestBasicLighting::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestBasicLighting::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        {
            // --------------------
            // Model View Projection
            // --------------------
            m_Proj = glm::perspective(glm::radians(m_Camera->GetFOV()), m_Camera->GetAspectRatio(), 0.1f, 100.f);
            m_View = glm::lookAt(m_Camera->GetPosition(), m_Camera->GetTarget(), m_Camera->GetUp());
            glm::mat4 model = glm::mat4(1.0f);

            // --------------------
            // Draw object
            // --------------------
            m_ObjShader->Bind();
            m_ObjShader->SetUniformMat4("model", model);
            m_ObjShader->SetUniformMat4("view", m_View);
            m_ObjShader->SetUniformMat4("projection", m_Proj);

            renderer.Draw(*m_Obj_VAO, *m_IBO, *m_ObjShader);

            // --------------------
            // Draw light
            // --------------------
            m_LightShader->Bind();
            model = glm::translate(model, m_LightPosition);
            model = glm::scale(model, glm::vec3(0.2f));
            m_LightShader->SetUniformMat4("model", model);
            m_LightShader->SetUniformMat4("view", m_View);
            m_LightShader->SetUniformMat4("projection", m_Proj);

            renderer.Draw(*m_Light_VAO, *m_IBO, *m_LightShader);
        }
    }

    void TestBasicLighting::OnImGuiRender()
    {
        float fov = m_Camera->GetFOV();
        if (ImGui::SliderFloat("FOV", &fov, 0.0f, 180.0f))
            m_Camera->SetFOV(fov);

        float aspectRatio = m_Camera->GetAspectRatio();
        if (ImGui::SliderFloat("AspectRatio", &aspectRatio, 0.0f, 10.0f))
            m_Camera->SetAspectRatio(aspectRatio);

        glm::vec3 camPos = m_Camera->GetPosition();
        if (ImGui::SliderFloat("CamPosX", &camPos.x, 0.0f, 10.0f))
            m_Camera->SetPositionX(camPos.x);
        if (ImGui::SliderFloat("CamPosY", &camPos.y, 0.0f, 10.0f))
            m_Camera->SetPositionY(camPos.y);
        if (ImGui::SliderFloat("CamPosZ", &camPos.z, 0.0f, 10.0f))
            m_Camera->SetPositionZ(camPos.z);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestBasicLighting::ProcessInput(float deltaTime)
    {
        // m_Camera->ProcessKeyboardMovement(deltaTime);
        // m_Camera->ProcessMouseMovement();
        // m_Camera->ProcessMouseScroll();
    }
}