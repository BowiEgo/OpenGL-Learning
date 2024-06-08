#include "11_TestModel.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGuiComponents/ToggleButton.h"

#include "Input.h"
#include "KeyCodes.h"

#include "Core/Material/BasicMaterial.h"
#include "Core/Material/StandardMaterial.h"

namespace test {
    TestModel::TestModel(GLFWwindow* window)
      : Test(window)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
        // --------------------
        // Scene
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
        m_DirectionalLight = std::make_shared<DirectionalLight>();
        m_Scene->Add(m_DirectionalLight);
        
        for (unsigned int i = 0; i < m_PointLightPositions.size(); i++)
        {
            std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(m_PointLightPositions[i]);
            m_PointLights.push_back(pointLight);
            m_Scene->Add(pointLight);
        }

        m_SpotLight = std::make_shared<SpotLight>();
        m_Scene->Add(m_SpotLight);

        // --------------------
        // Import models
        // --------------------
        m_ModelMichelle = std::make_shared<Model>("../res/models/michelle/michelle.obj");
        m_ModelMichelle->Translate(0.0f, 0.513f, 0.0f);
        m_Scene->Add(m_ModelMichelle);
        // --------------------
        // Container
        // --------------------
        // texture
        std::shared_ptr<Texture2D> diffuseTexture = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/container2.png");
        std::shared_ptr<Texture2D> specularTexture = std::make_shared<Texture2D>("Texture_Specular", "../res/textures/container2_specular.png");
        // material
        std::shared_ptr<StandardMaterial> containerMaterial = std::make_shared<StandardMaterial>();
        containerMaterial->SetDiffuseTexture(diffuseTexture);
        containerMaterial->SetSpecularTexture(specularTexture);
        // mesh
        for (unsigned int i = 0; i < m_ObjPositions.size(); i++)
        {
            std::shared_ptr<Mesh> containerMesh = std::make_shared<Mesh>(std::make_shared<BoxGeometry>(), containerMaterial);
            containerMesh->SetPosition(m_ObjPositions[i]);
            float angle = 20.0f * i;
            std::pair<float, glm::vec3> rotation = { glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f) };
            containerMesh->SetRotation(rotation);

            m_Scene->Add(containerMesh);
        }
        // --------------------
        // LightCube
        // --------------------
        // material
        std::shared_ptr<BasicMaterial> lightCubeMaterial = std::make_shared<BasicMaterial>();
        lightCubeMaterial->SetColor({ 1.0f, 1.0f, 1.0f });
        // mesh
        for (unsigned int i = 0; i < m_PointLightPositions.size(); i++)
        {
            std::shared_ptr<Mesh> m_LightCubeGeometryMesh = std::make_shared<Mesh>(std::make_shared<BoxGeometry>(), lightCubeMaterial);
            m_LightCubeGeometryMesh->SetPosition(m_PointLightPositions[i]);
            m_LightCubeGeometryMesh->SetScale(0.2f, 0.2f, 0.2f);

            m_Scene->Add(m_LightCubeGeometryMesh);
        }
    }

    TestModel::~TestModel()
    {
        GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
        delete m_Scene;
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


        for (unsigned int i = 0; i < m_Scene->GetPointLights().size(); i++)
        {
            m_Scene->GetPointLights()[i]->SetEnabled(m_IsPointLightsEnabled);
        }

        m_Scene->Update();

        // float lightColor[3] = {
        //     (m_Light->GetLightAmbient()[0] + m_Light->GetLightDiffuse()[0] + m_Light->GetLightSpecular()[0]) / 2.0f,
        //     (m_Light->GetLightAmbient()[1] + m_Light->GetLightDiffuse()[1] + m_Light->GetLightSpecular()[1]) / 2.0f,
        //     (m_Light->GetLightAmbient()[2] + m_Light->GetLightDiffuse()[2] + m_Light->GetLightSpecular()[2]) / 2.0f,
        // };

    }

    void TestModel::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        // ImGui::Bullet();ImGui::Text("Light");
        // ImGui::SliderFloat3("Ambient##Light", m_Light->GetLightAmbient(), 0.0f, 1.0f);
        // ImGui::SliderFloat3("Diffuse##Light", m_Light->GetLightDiffuse(), 0.0f, 1.0f);
        // ImGui::SliderFloat3("Specular##Light", m_Light->GetLightSpecular(), 0.0f, 1.0f);

        ImGui::Bullet();ImGui::Text("Directional Light");
        ImGui::SameLine();ImGui::ToggleButton("IsEnabled##DirectionalLight", m_DirectionalLight->GetIsEnabled());
        if (m_DirectionalLight->GetIsEnabled())
            ImGui::SliderFloat3("Direction##DirectionalLight", m_DirectionalLight->GetDirection(), -glm::pi<float>(), glm::pi<float>());
        
        ImGui::Bullet();ImGui::Text("Point Light");
        ImGui::SameLine();ImGui::ToggleButton("IsEnabled##PointLight", &m_IsPointLightsEnabled);

        ImGui::Bullet();ImGui::Text("Spot Light");
        ImGui::SameLine();ImGui::ToggleButton("IsEnabled##SpotLight", m_SpotLight->GetIsEnabled());
        if (m_SpotLight->GetIsEnabled())
        {
            float* spotLightPhi = m_SpotLight->GetPhi();
            float* spotLightSoftEdge = m_SpotLight->GetSoftEdge();
            if (ImGui::SliderFloat("Phi##SpotLight", spotLightPhi, 0.0f, 180.0f))
            {
                m_SpotLight->UpdateSpotLightCutOff();
            }
            if (ImGui::SliderFloat("SoftEdgePhi##SpotLight", spotLightSoftEdge, 0.0f, 10.0f))
            {
                m_SpotLight->UpdateSpotLightCutOff();
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
