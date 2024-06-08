#include "Core/Scene.h"

#include "FileSystem/FileSystem.h"
#include "Core/Material/StandardMaterial.h"

Scene* Scene::s_Instance = nullptr;

Scene* Scene::Create()
{
    s_Instance = new Scene();

    std::string basicVertSrc = FileSystem::ReadFile("../res/shaders/LightCube.vert");
    std::string basicFragSrc = FileSystem::ReadFile("../res/shaders/LightCube.frag");
    s_Instance->m_BasicShader = std::make_unique<Shader>(basicVertSrc, basicFragSrc);
    s_Instance->m_BasicShader->Bind();
    s_Instance->m_BasicShader->SetUniformVec3("u_Color", { 1.0f, 0.0f, 0.0f });

    std::string standardVertSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.vert");
    std::string standardFragSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.frag");
    s_Instance->m_StandardShader = std::make_shared<Shader>(standardVertSrc, standardFragSrc);

    return s_Instance;
}

void Scene::Add(std::shared_ptr<Camera> camera)
{
    s_Instance->m_Cameras.push_back(camera);
    s_Instance->m_CurrentCamera = camera;
}

void Scene::Add(std::shared_ptr<DirectionalLight> light)
{
    s_Instance->m_DirectionalLights.push_back(light);
    light->SetID(s_Instance->m_DirectionalLights.size() - 1);
}

void Scene::Add(std::shared_ptr<PointLight> light)
{
    s_Instance->m_PointLights.push_back(light);
    light->SetID(s_Instance->m_PointLights.size() - 1);
}

void Scene::Add(std::shared_ptr<SpotLight> light)
{
    s_Instance->m_SpotLights.push_back(light);
    light->SetID(s_Instance->m_SpotLights.size() - 1);
}

void Scene::Add(std::shared_ptr<Mesh> mesh)
{
    s_Instance->m_Meshes.push_back(mesh);
}

void Scene::Add(std::shared_ptr<Model> model)
{
    s_Instance->m_Models.push_back(model);
}

void Scene::Update()
{
    glm::mat4 proj = m_CurrentCamera->GetProjMatrix();
    glm::mat4 view = m_CurrentCamera->GetViewMatrix();
    glm::mat4 model(1.0f);

    m_BasicShader->Bind();
    m_BasicShader->SetUniformMat4("projectionMatrix", proj);
    m_BasicShader->SetUniformMat4("viewMatrix",       view);
    m_BasicShader->SetUniformMat4("modelMatrix",      model);

    m_StandardShader->Bind();
    m_StandardShader->SetUniformMat4("projectionMatrix", proj);
    m_StandardShader->SetUniformMat4("viewMatrix",       view);
    m_StandardShader->SetUniformMat4("modelMatrix",      model);
    m_StandardShader->SetUniformVec3("u_CameraPosition", m_CurrentCamera->GetPosition());

    for (unsigned int i = 0; i < m_DirectionalLights.size(); i++)
    {
        m_DirectionalLights[i]->Update((s_Instance->m_StandardShader).get());
    }

    for (unsigned int i = 0; i < m_PointLights.size(); i++)
    {
        m_StandardShader->SetUniform1i("u_NR_PointLights",  m_PointLights.size());
        m_PointLights[i]->Update((s_Instance->m_StandardShader).get());
    }

    for (unsigned int i = 0; i < m_SpotLights.size(); i++)
    {
        m_SpotLights[i]->Update((s_Instance->m_StandardShader).get());
    }

    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        glm::mat4 model(1.0f);
        glm::vec3 modelTranslate(0.0f);
        glm::vec3 modelScale(1.0f);
        modelTranslate.x = m_Meshes[i]->GetPosition()[0];
        modelTranslate.y = m_Meshes[i]->GetPosition()[1];
        modelTranslate.z = m_Meshes[i]->GetPosition()[2];

        modelScale.x = m_Meshes[i]->GetScale()[0];
        modelScale.y = m_Meshes[i]->GetScale()[1];
        modelScale.z = m_Meshes[i]->GetScale()[2];
        
        model = glm::translate(model, modelTranslate);
        model = glm::scale(model, modelScale);
        model = glm::rotate(model, m_Meshes[i]->GetRotation().first, m_Meshes[i]->GetRotation().second);

        Material* material = m_Meshes[i]->GetMaterial().get();
        switch (material->GetType())
        {
        case MATERIAL_TYPE_BASIC:
            m_BasicShader->Bind();
            m_BasicShader->SetUniformMat4("modelMatrix", model);
            m_Meshes[i]->Draw(m_BasicShader.get());
            break;
        case MATERIAL_TYPE_STANDARD:
            m_StandardShader->Bind();
            m_StandardShader->SetUniformMat4("modelMatrix", model);
            dynamic_cast<StandardMaterial*>(material)->BindTexture();
            m_Meshes[i]->Draw(m_StandardShader.get());
            break;
        }
    }

    for (unsigned int i = 0; i < m_Models.size(); i++)
    {
        glm::mat4 model(1.0f);
        glm::vec3 modelTranslate(0.0f);
        modelTranslate.x = m_Models[i]->GetTranslate()[0];
        modelTranslate.y = m_Models[i]->GetTranslate()[1];
        modelTranslate.z = m_Models[i]->GetTranslate()[2];
        model = glm::translate(model, modelTranslate);
        m_StandardShader->Bind();
        m_StandardShader->SetUniformMat4("modelMatrix", model);
        m_Models[i]->Draw(m_StandardShader.get());
    }
}
