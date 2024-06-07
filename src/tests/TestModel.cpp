#include "TestModel.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGuiComponents/ToggleButton.h"

#include "Input.h"
#include "KeyCodes.h"

#include "BasicMaterial.h"
#include "StandardMaterial.h"

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

namespace test {
    TestModel::TestModel(GLFWwindow* window)
      : Test(window)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
        // --------------------
        // Camera
        // --------------------
        m_Camera = std::make_shared<Camera>();

        // --------------------
        // Light
        // --------------------
        m_Light = std::make_shared<Light>(m_Camera);

        // --------------------
        // Import models
        // --------------------
        m_ModelMichelle = std::make_unique<Model>("../res/models/michelle/michelle.obj", m_Camera, m_Light);
        m_ModelMichelle->Translate(0.0f, 0.513f, 0.0f);
        // m_ModelNanosuit = std::make_unique<Model>("../res/models/nanosuit/nanosuit.obj");
        // ModelOptions backpackModelOpts;
        // backpackModelOpts.FlipUVs = true;
        // m_ModelBackpack = std::make_unique<Model>("../res/models/backpack/backpack.obj", backpackModelOpts);

        // --------------------
        // Container
        // --------------------
        // mesh
        m_ContainerMesh = std::make_unique<Mesh>(std::make_shared<BoxGeometry>(), std::make_shared<StandardMaterial>(m_Light));
        // texture
        m_DiffuseTexture = std::make_unique<Texture2D>("../res/textures/container2.png");
        m_SpecularTexture = std::make_unique<Texture2D>("../res/textures/container2_specular.png");
        m_Light->GetShader()->SetUniform1i("u_Material.diffuse",   0);
        m_Light->GetShader()->SetUniform1i("u_Material.specular",  1);

        // --------------------
        // LightCube
        // --------------------
        // geometry
        m_LightCubMesh = std::make_unique<Mesh>(std::make_shared<BoxGeometry>(), std::make_shared<BasicMaterial>());
    }

    TestModel::~TestModel()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
    }

    void TestModel::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestModel::OnRender()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        float currentTime = glfwGetTime();

        m_Light->UpdateShader();

        float lightColor[3] = {
            (m_Light->GetLightAmbient()[0] + m_Light->GetLightDiffuse()[0] + m_Light->GetLightSpecular()[0]) / 2.0f,
            (m_Light->GetLightAmbient()[1] + m_Light->GetLightDiffuse()[1] + m_Light->GetLightSpecular()[1]) / 2.0f,
            (m_Light->GetLightAmbient()[2] + m_Light->GetLightDiffuse()[2] + m_Light->GetLightSpecular()[2]) / 2.0f,
        };

        m_DiffuseTexture->Bind();
        m_SpecularTexture->Bind(1);

        {
            // --------------------
            // Draw containers
            // --------------------
            glm::mat4 model(1.0f);
            for (unsigned int i = 0; i < m_ObjPositions.size(); i++)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, m_ObjPositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                m_Light->GetShader()->SetUniformMat4("modelMatrix", model);

                m_ContainerMesh->Draw();
            }

            // --------------------
            // Draw lightCubes
            // --------------------
            if (m_Light->GetPointLightIsEnabled())
            {
                glm::mat4 proj = m_Camera->GetProjMatrix();
                glm::mat4 view = m_Camera->GetViewMatrix();
                m_LightCubMesh->GetMaterial()->GetShader()->Bind();
                m_LightCubMesh->GetMaterial()->GetShader()->SetUniformMat4("projectionMatrix", proj);
                m_LightCubMesh->GetMaterial()->GetShader()->SetUniformMat4("viewMatrix", view);
                m_LightCubMesh->GetMaterial()->SetColor(lightColor);
                
                for (int i = 0; i < m_Light->GetPointLightPositions().size(); i++)
                {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, m_Light->GetPointLightPositions()[i]);
                    model = glm::scale(model, glm::vec3(0.2f));
                    m_LightCubMesh->GetMaterial()->GetShader()->SetUniformMat4("modelMatrix", model);
                    m_LightCubMesh->Draw();
                }
            }

            // --------------------
            // Draw models
            // --------------------
            m_ModelMichelle->Draw();
        }
    }

    void TestModel::OnImGuiRender()
    {
        ImGui::Bullet();ImGui::Text("Material");
        ImGui::SliderFloat("Shininess##Material", m_Light->GetMaterialShininess(), 0.0f, 256.0f);

        ImGui::Bullet();ImGui::Text("Light");
        ImGui::SliderFloat3("Ambient##Light", m_Light->GetLightAmbient(), 0.0f, 1.0f);
        ImGui::SliderFloat3("Diffuse##Light", m_Light->GetLightDiffuse(), 0.0f, 1.0f);
        ImGui::SliderFloat3("Specular##Light", m_Light->GetLightSpecular(), 0.0f, 1.0f);

        ImGui::Bullet();ImGui::Text("Directional Light");
        ImGui::SameLine();ImGui::ToggleButton("IsEnabled##DirectionalLight", m_Light->GetDirectionalLightIsEnabled());
        if (m_Light->GetDirectionalLightIsEnabled())
            ImGui::SliderFloat3("Direction##DirectionalLight", m_Light->GetDirectionalLightDir(), -glm::pi<float>(), glm::pi<float>());

        ImGui::Bullet();ImGui::Text("Point Light");
        ImGui::SameLine();ImGui::ToggleButton("IsEnabled##PointLight", m_Light->GetPointLightIsEnabled());

        ImGui::Bullet();ImGui::Text("Spot Light");
        ImGui::SameLine();ImGui::ToggleButton("IsEnabled##SpotLight", m_Light->GetSpotLightIsEnabled());
        if (m_Light->GetSpotLightIsEnabled())
        {
            float* spotLightPhi = m_Light->GetSpotLightPhi();
            float* spotLightSoftEdge = m_Light->GetSpotLight_SoftEdge();
            if (ImGui::SliderFloat("Phi##SpotLight", spotLightPhi, 0.0f, 180.0f))
            {
                m_Light->UpdateSpotLightCutOff();
            }
            if (ImGui::SliderFloat("SoftEdgePhi##SpotLight", spotLightSoftEdge, 0.0f, 10.0f))
            {
                m_Light->UpdateSpotLightCutOff();
            }
        }

        ImGui::Bullet();ImGui::Text("Model Michelle");
        ImGui::SliderFloat3("Translate##Michelle", m_ModelMichelle->GetTranslate(), -5.0f, 5.0f);
    }

    void TestModel::ProcessInput(float deltaTime)
    {
        m_Camera->ProcessKeyboardMovement(deltaTime);
        m_Camera->ProcessMouseMovement();
        m_Camera->ProcessMouseScroll();
    }

    void TestModel::SetCameraAspectRatio(float aspectRatio)
    {
        m_Camera->SetAspectRatio(aspectRatio);
    }

    void TestModel::EnableCameraControll()
    {
        m_Camera->EnableControll();
    }

    void TestModel::DisableCameraControll()
    {
        m_Camera->DisableControll();
    }
}
