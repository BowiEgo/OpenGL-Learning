#include "Scene.h"

Scene* Scene::s_Instance = nullptr;
MaterialManager* Scene::s_MaterialManager = nullptr;

Scene::Scene()
{
}

Scene::~Scene()
{
    delete s_MaterialManager;
}

Scene *Scene::Create()
{
    s_Instance = new Scene();
    s_MaterialManager = MaterialManager::Create();

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
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        m_Meshes[i]->GetMaterial()->UpdateShader(
            m_Meshes[i]->GetPosition(),
            m_Meshes[i]->GetScale(),
            m_Meshes[i]->GetRotation());

        m_Meshes[i]->Draw();

        Scene::GetMaterialManager()->GetOutlineMaterial()->UpdateShader(
            m_Meshes[i]->GetPosition(),
            m_Meshes[i]->GetScale(),
            m_Meshes[i]->GetRotation());

        m_Meshes[i]->DrawOutline();
    }

    for (unsigned int i = 0; i < m_Models.size(); i++)
    {
        m_Models[i]->Draw();
    }
}
