#include "15_TestFaceCulling.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGuiComponents/ToggleButton.h"

#include "Input.h"
#include "KeyCodes.h"

#include "Core/Geometry/GrassGeometry.h"
#include "Core/Geometry/PlaneGeometry.h"
#include "Core/Geometry/BoxGeometry.h"
#include "Core/Material/StandardMaterial.h"

#include "Core/InstanceMesh.h"

namespace test {
    TestFaceCulling::TestFaceCulling(GLFWwindow* window)
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
        // Import models
        // --------------------

        // --------------------
        // Plane
        // --------------------
        // texture
        Ref<Texture2D> diffuseTexture_plane = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/metal.png");
        diffuseTexture_plane->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_plane->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        Ref<StandardMaterial> material_plane = std::make_shared<StandardMaterial>();
        material_plane->AddTexture(diffuseTexture_plane);
        // mesh
        m_Mesh_Floor = std::make_shared<Mesh>(std::make_shared<PlaneGeometry>(), material_plane);
        m_Mesh_Floor->SetPosition(0.0f, -0.01f, 0.0f);
        m_Mesh_Floor->Cull_Face = m_CullFaceOption_Floor;
        m_Scene->Add(m_Mesh_Floor);

        // --------------------
        // Container
        // --------------------
        std::vector<glm::vec3> containerPositions = {
            glm::vec3( 0.0f,  0.0f,   0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f,  -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f,  -3.5f),
            glm::vec3(-1.7f,  3.0f,  -7.5f),
            glm::vec3( 1.3f, -2.0f,  -2.5f),
            glm::vec3( 1.5f,  2.0f,  -2.5f),
            glm::vec3( 1.5f,  0.2f,  -1.5f),
            glm::vec3(-1.3f,  1.0f,  -1.5f)  
        };
        // texture
        Ref<Texture2D> diffuseTexture_container = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/marble.jpg");
        // material
        Ref<StandardMaterial> material_container = std::make_shared<StandardMaterial>();
        material_container->AddTexture(diffuseTexture_container);
        // mesh
        m_Mesh_Container = std::make_shared<InstanceMesh>(
            std::make_shared<BoxGeometry>(),
            material_container, containerPositions.size());

        for (unsigned int i = 0; i < containerPositions.size(); i++)
        {
            m_Mesh_Container->SetPosition(i, containerPositions[i]);
            float angle = 20.0f * i;
            std::pair<float, glm::vec3> rotation = { glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f) };
            m_Mesh_Container->SetRotation(i, rotation);
        }
        m_Mesh_Container->Cull_Face = m_CullFaceOption_Container;
        m_Scene->Add(m_Mesh_Container);

        // --------------------
        // Vegetation
        // --------------------
        std::vector<glm::vec3> vegetation;
        vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.48f));
        vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.51f));
        vegetation.push_back(glm::vec3( 1.2f,  0.0f,   0.7f));
        vegetation.push_back(glm::vec3(-1.5f,  0.0f,   1.1f));
        vegetation.push_back(glm::vec3( 1.1f,  0.0f,   2.6f));
        // texture
        TextureOptions grassTexOpts = TextureOptions();
        grassTexOpts.flip = false;
        Ref<Texture2D> diffuseTexture_grass = std::make_shared<Texture2D>("Texture_Diffuse", "../res/textures/grass.png", grassTexOpts);
        diffuseTexture_grass->SetWrapping(GL_TEXTURE_WRAP_S, GL_REPEAT);
        diffuseTexture_grass->SetWrapping(GL_TEXTURE_WRAP_T, GL_REPEAT);
        // material
        Ref<StandardMaterial> material_grass = std::make_shared<StandardMaterial>();
        material_grass->AddTexture(diffuseTexture_grass);
        material_grass->Discard_Transparent = true;
        // mesh
        m_Mesh_Grass = std::make_shared<InstanceMesh>(
            std::make_shared<GrassGeometry>(),
            material_grass, vegetation.size());

        for (unsigned int i = 0; i < vegetation.size(); i++)
        {
            m_Mesh_Grass->SetPosition(i, vegetation[i]);
        }

        m_Mesh_Grass->Cull_Face = m_CullFaceOption_Grass;
        m_Scene->Add(m_Mesh_Grass);
    }

    TestFaceCulling::~TestFaceCulling()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestFaceCulling::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestFaceCulling::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        float currentTime = glfwGetTime();

        m_Mesh_Floor->Cull_Face = m_CullFaceOption_Floor;
        m_Mesh_Container->Cull_Face = m_CullFaceOption_Container;
        m_Mesh_Grass->Cull_Face = m_CullFaceOption_Grass;

        m_Mesh_Floor->GetMaterial()->Wireframe_Enabled = m_Wireframe_Enabled_Floor;
        m_Mesh_Container->GetMaterial()->Wireframe_Enabled = m_Wireframe_Enabled_Container;
        m_Mesh_Grass->GetMaterial()->Wireframe_Enabled = m_Wireframe_Enabled_Grass;

        m_Scene->Draw();
    }

    void TestFaceCulling::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        const char* items[] = { "CULL_FACE_NONE", "CULL_FACE_BACK", "CULL_FACE_FRONT", "CULL_FACE_FRONT_AND_BACK" };

        static int item_current_floor = 2;
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Floor", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("Wireframe");
            ImGui::SameLine();ImGui::ToggleButton("Wireframe##Floor", &m_Wireframe_Enabled_Floor);

            ImGui::Text("CullFace");ImGui::SameLine();
            if (ImGui::Combo("##Floor", &item_current_floor, items, IM_ARRAYSIZE(items)))
            {
                m_CullFaceOption_Floor = static_cast<CullFaceOption>(item_current_floor);
            }
        }

        static int item_current_container = 1;
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Container", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("Wireframe");
            ImGui::SameLine();ImGui::ToggleButton("Wireframe##Container", &m_Wireframe_Enabled_Container);

            ImGui::Text("CullFace");ImGui::SameLine();
            if (ImGui::Combo("##Container", &item_current_container, items, IM_ARRAYSIZE(items)))
            {
                m_CullFaceOption_Container = static_cast<CullFaceOption>(item_current_container);
            }
        }

        static int item_current_grass = 0;
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Container", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("Wireframe");
            ImGui::SameLine();ImGui::ToggleButton("Wireframe##Grass", &m_Wireframe_Enabled_Grass);
            
            ImGui::Text("CullFace");ImGui::SameLine();
            if (ImGui::Combo("##Grass", &item_current_grass, items, IM_ARRAYSIZE(items)))
            {
                m_CullFaceOption_Grass = static_cast<CullFaceOption>(item_current_grass);
            }
        }
    }
}
