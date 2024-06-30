#include "29_TestDeferredShading.h"

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
#include "Core/Material/CubemapMaterial.h"

#include "Core/InstanceMesh.h"

namespace test {
    TestDeferredShading::TestDeferredShading(GLFWwindow* window)
      : Test(window)
    {
        // --------------------
        // Setup
        // --------------------
        m_Scene = Scene::Create();
        // --------------------
        // Camera
        // --------------------
        m_Camera = std::make_shared<PerspectiveCamera>();
        m_Camera->SetPosition({ 3.4f, 1.7f, 8.0f });
        m_Scene->Add(m_Camera);
        // --------------------
        // Light
        // --------------------
        const unsigned int NR_LIGHTS = 32;
        std::vector<glm::vec3> lightPositions;
        std::vector<glm::vec3> lightColors;
        srand(13);
        for (unsigned int i = 0; i < NR_LIGHTS; i++)
        {
            // Calculate slightly random offsets
            float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
            float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
            float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
            lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
            // Also calculate random color
            float rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
            float gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
            float bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
            lightColors.push_back(glm::vec3(rColor, gColor, bColor));
        }
        // PointLight
        for (int i = 0; i < NR_LIGHTS; i++)
        {
            Ref<PointLight> light = std::make_shared<PointLight>();
            light->SetPosition(lightPositions[i]);
            light->SetColor(lightColors[i]);
            m_PointLights.push_back(light);
            m_Scene->Add(light);

            // Light cube
            Ref<BoxGeometry> geometry_lightCube = std::make_shared<BoxGeometry>();
            // material
            Ref<BasicMaterial> material_lightCube = std::make_shared<BasicMaterial>();
            material_lightCube->SetColor(lightColors[i]);
            Ref<Mesh> lightCube = std::make_shared<Mesh>(geometry_lightCube, material_lightCube);
            lightCube->SetPosition(lightPositions[i]);
            lightCube->SetScale(glm::vec3(0.2f));
            m_Mesh_LightCubes.push_back(lightCube);

            m_Scene->Add(lightCube);
        }
        // --------------------
        // Model
        // --------------------
        m_Model_Nanosuit = std::make_shared<Model>("../res/models/nanosuit/nanosuit.obj");
        // matrices
        unsigned int amount = m_Model_Positions.size();
        for(unsigned int i = 0; i < amount; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_Model_Positions[i]);

            float scale = 0.2f;
            model = glm::scale(model, glm::vec3(scale));

            float rotAngle = 0.0f;
            model = glm::rotate(model, glm::radians(rotAngle), glm::vec3(0.0f, 0.0f, 1.0f));

            m_ModelMatrices[i] = model;
        }
        // instance VBO
        m_InstanceVBO_Nanosuit = std::make_shared<VertexBuffer>(&m_ModelMatrices[0], sizeof(glm::mat4) * amount);
        std::vector<Ref<Mesh>> meshes_nanosuit = m_Model_Nanosuit->GetMeshes();
        GLsizei vec4Size = sizeof(glm::vec4);
        for (unsigned int i = 0; i < meshes_nanosuit.size(); i++)
        {
            dynamic_cast<StandardMaterial*>(meshes_nanosuit[i]->GetMaterial().get())->BindFragmentShader("../res/shaders/GBuffer.frag");
            dynamic_cast<StandardMaterial*>(meshes_nanosuit[i]->GetMaterial().get())->Setup();
            meshes_nanosuit[i]->SetDrawWay(DrawWay::Instanced, amount);
            meshes_nanosuit[i]->GetMaterial()->UpdateShaderUniform("u_Is_Instance", true);
            meshes_nanosuit[i]->AddInstanceVBO(m_InstanceVBO_Nanosuit, 4, vec4Size, 4);
        }
        // --------------------
        // GBuffer
        // --------------------
        m_PostprocessingGBuffer = std::make_shared<PostprocessingGBuffer>();
    }

    TestDeferredShading::~TestDeferredShading()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        delete m_Scene;
    }

    void TestDeferredShading::OnUpdate(float deltaTime)
    {
        ProcessInput(deltaTime);
    }

    void TestDeferredShading::OnRender()
    {
        // --------------------
        // Postprocessing GBuffer
        // --------------------
        m_PostprocessingGBuffer->Bind();
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        m_Scene->Draw(m_Model_Nanosuit.get());
        m_PostprocessingGBuffer->Unbind();

        // --------------------
        // Draw stage
        // --------------------
        Framebuffer* viewport;
        if (FramebufferManager::s_SMAA_Enabled)
        {
            viewport = FramebufferManager::GetByTag("viewport").get();
        }
        else
        {
            viewport = FramebufferManager::GetByTag("nonMultisample_viewport").get();
        }

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_PostprocessingGBuffer->GetGBuffer()->GetID());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, viewport->GetID()); // Write to default framebuffer
        // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
        // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
        // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
        glBlitFramebuffer(0, 0, m_PostprocessingGBuffer->GetGBuffer()->GetSpecification().Width,
            m_PostprocessingGBuffer->GetGBuffer()->GetSpecification().Height, 0, 0, viewport->GetSpecification().Width,
            viewport->GetSpecification().Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        viewport->Bind();
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        OnViewPortResize();

        // m_Scene->Draw(m_Model_Nanosuit.get());
        m_Scene->Draw(m_PostprocessingGBuffer->GetDeferredShadingMesh());
        m_Scene->Draw();
        m_Scene->Draw(m_PostprocessingGBuffer->GetPositionMesh());
        m_Scene->Draw(m_PostprocessingGBuffer->GetNormalMesh());
        m_Scene->Draw(m_PostprocessingGBuffer->GetAlbedoMesh());
        m_Scene->Draw(m_PostprocessingGBuffer->GetSpecularMesh());

        viewport->Unbind();
    }

    void TestDeferredShading::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("CameraPosition %.3f, %.3f, %.3f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
    }
}
