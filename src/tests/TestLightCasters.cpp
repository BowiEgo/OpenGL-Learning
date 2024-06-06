#include "TestLightCasters.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGuiComponents/ToggleButton.h"

#include "Input.h"
#include "KeyCodes.h"

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

namespace test {
    TestLightCasters::TestLightCasters(GLFWwindow* window)
      : Test(window)
    {
        GLCall(glEnable(GL_DEPTH_TEST));

        // --------------------
        // Camera
        // --------------------
        m_Camera = std::make_unique<Camera>();

        // --------------------
        // Model datas
        // --------------------
        float vertices[] = {
            // --- positions ---//---- normals ------//-- coords --
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };
        
        // --------------------
        // Model
        // --------------------
        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);

        // Object model
        m_Obj_VAO = std::make_unique<VertexArray>();
        m_Obj_VAO->AddBuffer(*m_VBO, layout);

        // LightCube model
        m_LightCube_VAO = std::make_unique<VertexArray>();
        m_LightCube_VAO->AddBuffer(*m_VBO, layout);

        // --------------------
        // Texture
        // --------------------
        m_DiffuseTexture = std::make_unique<Texture>("../res/textures/container2.png");
        m_SpecularTexture = std::make_unique<Texture>("../res/textures/container2_specular.png");

        // --------------------
        // Shader
        // --------------------
        // Object shader
        std::string vertexSrc = FileSystem::ReadFile("../res/shaders/LightCasters.vert");
        std::string fragSrc = FileSystem::ReadFile("../res/shaders/LightCasters.frag");
        m_ObjShader = std::make_unique<Shader>(vertexSrc, fragSrc);

        // Shader uniforms
        m_ObjShader->Bind();
        m_ObjShader->SetUniform1i("u_Material.diffuse",   0);
        m_ObjShader->SetUniform1i("u_Material.specular",  1);
        m_ObjShader->SetUniform1i("u_Material.emmision",  2);
        m_ObjShader->SetUniform1f("u_Material.shininess", m_MaterialShininess);

        // LightCube shader
        std::string lightCubeVertexSrc = FileSystem::ReadFile("../res/shaders/LightCube.vert");
        std::string lightCubeFragSrc = FileSystem::ReadFile("../res/shaders/LightCube.frag");
        m_PointLightCubeShader = std::make_unique<Shader>(lightCubeVertexSrc, lightCubeFragSrc);
        m_PointLightCubeShader->Bind();
        m_PointLightCubeShader->SetUniformVec3("u_Color", { 1.0f, 1.0f, 1.0f });

        m_SpotLightCubeShader = std::make_unique<Shader>(lightCubeVertexSrc, lightCubeFragSrc);
    }

    TestLightCasters::~TestLightCasters()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
    }

    void TestLightCasters::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestLightCasters::OnRender()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        float currentTime = glfwGetTime();

        float lightColor[3] = {
            (m_LightAmbient[0] + m_LightDiffuse[0] + m_LightSpecular[0]) / 3.0f,
            (m_LightAmbient[1] + m_LightDiffuse[1] + m_LightSpecular[1]) / 3.0f,
            (m_LightAmbient[2] + m_LightDiffuse[2] + m_LightSpecular[2]) / 3.0f,
        };

        m_DiffuseTexture->Bind();
        m_SpecularTexture->Bind(1);

        {
            // --------------------
            // Model View Projection
            // --------------------
            glm::mat4 proj = m_Camera->GetProjMatrix();
            glm::mat4 view = m_Camera->GetViewMatrix();
            glm::mat4 model(1.0f);
            // --------------------
            // Draw object
            // --------------------
            m_ObjShader->Bind();
            m_ObjShader->SetUniformMat4("projectionMatrix", proj);
            m_ObjShader->SetUniformMat4("viewMatrix",       view);
            m_ObjShader->SetUniformVec3("u_CameraPosition", m_Camera->GetPosition());

            m_ObjShader->  SetUniform1f("u_Material.shininess",        m_MaterialShininess);

            m_ObjShader->SetUniformBool("u_DirectionalLight.enable",    m_IsEnabled_DirectionalLight);
            m_ObjShader->SetUniformVec3("u_DirectionalLight.ambient",   m_LightAmbient);
            m_ObjShader->SetUniformVec3("u_DirectionalLight.diffuse",   m_LightDiffuse);
            m_ObjShader->SetUniformVec3("u_DirectionalLight.specular",  m_LightSpecular);
            m_ObjShader->SetUniformVec3("u_DirectionalLight.direction", m_DirectionalLightDir);

            m_ObjShader->SetUniformBool("u_PointLight.enable",    m_IsEnabled_PointLight);
            m_ObjShader->SetUniformVec3("u_PointLight.ambient",   m_LightAmbient);
            m_ObjShader->SetUniformVec3("u_PointLight.diffuse",   m_LightDiffuse);
            m_ObjShader->SetUniformVec3("u_PointLight.specular",  m_LightSpecular);
            m_ObjShader->SetUniformVec3("u_PointLight.position",  m_PointLightPos);
            m_ObjShader->  SetUniform1f("u_PointLight.constant",  1.0f);
            m_ObjShader->  SetUniform1f("u_PointLight.linear",    0.09f);
            m_ObjShader->  SetUniform1f("u_PointLight.quadratic", 0.032f);

            m_ObjShader->SetUniformBool("u_SpotLight.enable",      m_IsEnabled_SpotLight);
            m_ObjShader->SetUniformVec3("u_SpotLight.ambient",     m_LightAmbient);
            m_ObjShader->SetUniformVec3("u_SpotLight.diffuse",     m_LightDiffuse);
            m_ObjShader->SetUniformVec3("u_SpotLight.specular",    m_LightSpecular);
            m_ObjShader->SetUniformVec3("u_SpotLight.position",    m_Camera->GetPosition());
            m_ObjShader->SetUniformVec3("u_SpotLight.direction",   m_Camera->GetFront());
            m_ObjShader->  SetUniform1f("u_SpotLight.cutOff",      m_SpotLightCutOff);
            m_ObjShader->  SetUniform1f("u_SpotLight.outerCutOff", m_SpotLightOuterCutOff);
            m_ObjShader->  SetUniform1f("u_SpotLight.constant",  1.0f);
            m_ObjShader->  SetUniform1f("u_SpotLight.linear",    0.09f);
            m_ObjShader->  SetUniform1f("u_SpotLight.quadratic", 0.032f);

            for (unsigned int i = 0; i < m_ObjPositions.size(); i++)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, m_ObjPositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                
                m_ObjShader->SetUniformMat4("modelMatrix",          model);

                renderer.Draw(*m_ObjShader, *m_Obj_VAO);
            }

            // --------------------
            // Draw lightCube
            // --------------------
            if (m_IsEnabled_PointLight)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(m_PointLightPos[0], m_PointLightPos[1], m_PointLightPos[2]));
                model = glm::scale(model, glm::vec3(0.2f));
                m_PointLightCubeShader->Bind();
                m_PointLightCubeShader->SetUniformMat4("projectionMatrix", m_Camera->GetProjMatrix());
                m_PointLightCubeShader->SetUniformMat4("viewMatrix", m_Camera->GetViewMatrix());
                m_PointLightCubeShader->SetUniformMat4("modelMatrix", model);
                m_PointLightCubeShader->SetUniformVec3("u_Color", lightColor);

                renderer.Draw(*m_PointLightCubeShader, *m_LightCube_VAO);
            }
        }
    }

    void TestLightCasters::OnImGuiRender()
    {
        ImGui::SeparatorText("Camera");
        float fov = m_Camera->GetFOV();
        if (ImGui::SliderFloat("FOV", &fov, 0.0f, 180.0f))
            m_Camera->SetFOV(fov);

        ImGui::Bullet();ImGui::Text("Material");
        ImGui::SliderFloat("Shininess##Material", &m_MaterialShininess, 0.0f, 256.0f);

        ImGui::Bullet();ImGui::Text("Light");
        ImGui::SliderFloat3("Ambient##Light", m_LightAmbient, 0.0f, 1.0f);
        ImGui::SliderFloat3("Diffuse##Light", m_LightDiffuse, 0.0f, 1.0f);
        ImGui::SliderFloat3("Specular##Light", m_LightSpecular, 0.0f, 1.0f);

        ImGui::Bullet();ImGui::Text("Directional Light");
        ImGui::SameLine();ImGui::ToggleButton("IsEnabled##DirectionalLight", &m_IsEnabled_DirectionalLight);
        if (m_IsEnabled_DirectionalLight)
            ImGui::SliderFloat3("Direction##DirectionalLight", m_DirectionalLightDir, -glm::pi<float>(), glm::pi<float>());

        ImGui::Bullet();ImGui::Text("Point Light");
        ImGui::SameLine();ImGui::ToggleButton("IsEnabled##PointLight", &m_IsEnabled_PointLight);
        if (m_IsEnabled_PointLight)
            ImGui::SliderFloat3("Position##PointLight", m_PointLightPos, -5.0f, 5.0f);

        ImGui::Bullet();ImGui::Text("Spot Light");
        ImGui::SameLine();ImGui::ToggleButton("IsEnabled##SpotLight", &m_IsEnabled_SpotLight);
        if (m_IsEnabled_SpotLight)
        {
            if (ImGui::SliderFloat("Phi##SpotLight", &m_SpotLightPhi, 0.0f, 180.0f))
            {
                m_SpotLightCutOff = glm::cos(glm::radians(m_SpotLightPhi));
                m_SpotLightOuterCutOff = glm::cos(glm::radians(m_SpotLightPhi + m_SpotLight_SoftEdge));
            }
            if (ImGui::SliderFloat("SoftEdgePhi##SpotLight", &m_SpotLight_SoftEdge, 0.0f, 10.0f))
            {
                m_SpotLightCutOff = glm::cos(glm::radians(m_SpotLightPhi));
                m_SpotLightOuterCutOff = glm::cos(glm::radians(m_SpotLightPhi + m_SpotLight_SoftEdge));
            }
        }
    }

    void TestLightCasters::ProcessInput(float deltaTime)
    {
        m_Camera->ProcessKeyboardMovement(deltaTime);
        m_Camera->ProcessMouseMovement();
        m_Camera->ProcessMouseScroll();
    }

    void TestLightCasters::SetCameraAspectRatio(float aspectRatio)
    {
        m_Camera->SetAspectRatio(aspectRatio);
    }

    void TestLightCasters::EnableCameraControll()
    {
        m_Camera->EnableControll();
    }

    void TestLightCasters::DisableCameraControll()
    {
        m_Camera->DisableControll();
    }
}
