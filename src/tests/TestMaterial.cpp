#include "TestMaterial.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include "KeyCodes.h"

namespace test {
    TestMaterial::TestMaterial(GLFWwindow* window)
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
        std::string vertexSrc = FileSystem::ReadFile("../res/shaders/Material.vert");
        std::string fragSrc = FileSystem::ReadFile("../res/shaders/Material.frag");
        m_ObjShader = std::make_unique<Shader>(vertexSrc, fragSrc);

        // Shader uniforms
        m_ObjShader->Bind();
        m_ObjShader->SetUniformVec3("u_Material.ambient",   m_MaterialAmbient);
        m_ObjShader->SetUniformVec3("u_Material.diffuse",   m_MaterialDiffuse);
        m_ObjShader->SetUniformVec3("u_Material.specular",  m_MaterialSpecular);
        m_ObjShader->  SetUniform1f("u_Material.shininess", m_MaterialShininess);
        m_ObjShader->SetUniformVec3("u_Light.ambient",  m_LightAmbient);
        m_ObjShader->SetUniformVec3("u_Light.diffuse",  m_LightDiffuse);
        m_ObjShader->SetUniformVec3("u_Light.specular", m_LightSpecular);

        // Light shader
        std::string lightVertexSrc = FileSystem::ReadFile("../res/shaders/LightCube.vert");
        std::string lightFragSrc = FileSystem::ReadFile("../res/shaders/LightCube.frag");
        m_LightShader = std::make_unique<Shader>(lightVertexSrc, lightFragSrc);
        m_LightShader->Bind();
        m_LightShader->SetUniformVec3("u_Color", { 1.0f, 1.0f, 1.0f });
    }

    TestMaterial::~TestMaterial()
    {
    }

    void TestMaterial::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestMaterial::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        float currentTime = glfwGetTime();

        float radius = 2.0f;
        m_LightPosition.x = sin(currentTime) * radius;
        m_LightPosition.z = cos(currentTime) * radius;

        // glm::vec3 lightColor = m_LightAmbient * m_LightDiffuse * m_LightSpecular;
        // lightColor.x = (sin(currentTime * 2.0f) + 1.0f) * 0.5;
        // lightColor.y = (sin(currentTime * 0.7f) + 1.0f) * 0.5;
        // lightColor.z = (sin(currentTime * 1.3f) + 1.0f) * 0.5;

        // glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f);
        // glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        float lightColor[3] = {
            (m_LightAmbient[0] + m_LightDiffuse[0] + m_LightSpecular[0]) / 3.0f,
            (m_LightAmbient[1] + m_LightDiffuse[1] + m_LightSpecular[1]) / 3.0f,
            (m_LightAmbient[2] + m_LightDiffuse[2] + m_LightSpecular[2]) / 3.0f,
        };

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
            m_ObjShader->SetUniformMat4("modelMatrix",          model);
            m_ObjShader->SetUniformMat4("viewMatrix",           m_View);
            m_ObjShader->SetUniformMat4("projectionMatrix",     m_Proj);
            m_ObjShader->SetUniformVec3("u_CameraPosition",     m_Camera->GetPosition());

            m_ObjShader->SetUniformVec3("u_Material.ambient",   m_MaterialAmbient);
            m_ObjShader->SetUniformVec3("u_Material.diffuse",   m_MaterialDiffuse);
            m_ObjShader->SetUniformVec3("u_Material.specular",  m_MaterialSpecular);
            m_ObjShader->  SetUniform1f("u_Material.shininess", m_MaterialShininess);

            m_ObjShader->SetUniformVec3("u_Light.position", m_LightPosition);
            m_ObjShader->SetUniformVec3("u_Light.ambient",  m_LightAmbient);
            m_ObjShader->SetUniformVec3("u_Light.diffuse",  m_LightDiffuse);
            m_ObjShader->SetUniformVec3("u_Light.specular", m_LightSpecular);

            renderer.Draw(*m_ObjShader, *m_Obj_VAO);

            // --------------------
            // Draw light
            // --------------------
            model = glm::translate(model, m_LightPosition);
            model = glm::scale(model, glm::vec3(0.2f));
            m_LightShader->Bind();
            m_LightShader->SetUniformMat4("modelMatrix", model);
            m_LightShader->SetUniformMat4("viewMatrix", m_View);
            m_LightShader->SetUniformMat4("projectionMatrix", m_Proj);
            m_LightShader->SetUniformVec3("u_Color", lightColor);

            renderer.Draw(*m_LightShader, *m_Light_VAO);
        }
    }

    void TestMaterial::OnImGuiRender()
    {
        ImGui::SeparatorText("Camera");
        float fov = m_Camera->GetFOV();
        if (ImGui::SliderFloat("FOV", &fov, 0.0f, 180.0f))
            m_Camera->SetFOV(fov);

        float aspectRatio = m_Camera->GetAspectRatio();
        if (ImGui::SliderFloat("AspectRatio", &aspectRatio, 0.0f, 10.0f))
            m_Camera->SetAspectRatio(aspectRatio);

        glm::vec3 camPos = m_Camera->GetPosition();
        if (ImGui::SliderFloat("CamPosX", &camPos.x, -10.0f, 10.0f))
            m_Camera->SetPositionX(camPos.x);
        if (ImGui::SliderFloat("CamPosY", &camPos.y, -10.0f, 10.0f))
            m_Camera->SetPositionY(camPos.y);
        if (ImGui::SliderFloat("CamPosZ", &camPos.z, -10.0f, 10.0f))
            m_Camera->SetPositionZ(camPos.z);

        ImGui::Bullet();ImGui::Text("Material attributes");
        ImGui::SliderFloat3("Ambient##Material", m_MaterialAmbient, 0.0f, 1.0f);
        ImGui::SliderFloat3("Diffuse##Material", m_MaterialDiffuse, 0.0f, 1.0f);
        ImGui::SliderFloat3("Specular##Material", m_MaterialSpecular, 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess##Material", &m_MaterialShininess, 0.0f, 256.0f);

        ImGui::Bullet();ImGui::Text("Light attributes");
        ImGui::SliderFloat3("Ambient##Light", m_LightAmbient, 0.0f, 1.0f);
        ImGui::SliderFloat3("Diffuse##Light", m_LightDiffuse, 0.0f, 1.0f);
        ImGui::SliderFloat3("Specular##Light", m_LightSpecular, 0.0f, 1.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestMaterial::ProcessInput(float deltaTime)
    {
    }
}