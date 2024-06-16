#include "Scene.h"

Scene* Scene::s_Instance = nullptr;
MaterialManager* Scene::s_MaterialManager = nullptr;
TextureCubemap* Scene::s_VoidTextureCubemap = nullptr;
Texture2D* Scene::s_VoidTexture2D = nullptr;

Scene::Scene()
{
}

Scene::~Scene()
{
    delete s_MaterialManager;
    delete s_VoidTextureCubemap;
    delete s_VoidTexture2D;
}

Scene *Scene::Create()
{
    s_Instance = new Scene();
    s_MaterialManager = MaterialManager::Create();
    s_VoidTextureCubemap = TextureCubemap::CreateVoidTexture("Texture_Environment");
    s_VoidTexture2D = Texture2D::CreateVoidTexture("Texture_Void");

    return s_Instance;
}


void Scene::SetSkybox(Ref<Mesh> skybox)
{
    m_Skybox = skybox;
}

void Scene::Add(std::shared_ptr<Camera> camera)
{
    s_Instance->m_Cameras.push_back(camera);
    s_Instance->m_CurrentCamera = camera;
    Shader::UpdateMatricesProj(m_CurrentCamera->GetProjMatrix());
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
    if (mesh->Is_Transparent)
    {
        s_Instance->m_Transparent_Meshes.push_back(mesh);
    }
    else
    {
        s_Instance->m_Meshes.push_back(mesh);
    }
}

void Scene::Add(std::shared_ptr<InstanceMesh> mesh)
{
    if (mesh->Is_Transparent)
    {
        s_Instance->m_Transparent_Meshes.push_back(mesh);
    }
    else
    {
        s_Instance->m_Meshes.push_back(mesh);
    }
}

void Scene::Add(std::shared_ptr<Model> model)
{
    s_Instance->m_Models.push_back(model);
}

void Scene::SetCurrentCamera(Ref<Camera> camera)
{
    m_CurrentCamera = camera;
}

void Scene::Draw()
{
    // --------------------
    // Update shader uniform blocks
    // --------------------
    Shader::UpdateMatricesView(m_CurrentCamera->GetViewMatrix());

    // --------------------
    // Draw meshes
    // --------------------
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        if (dynamic_cast<InstanceMesh*>(m_Meshes[i].get()))
        {
            dynamic_cast<InstanceMesh*>(m_Meshes[i].get())->Draw();
            dynamic_cast<InstanceMesh*>(m_Meshes[i].get())->DrawOutline();
        }
        else
        {
            m_Meshes[i]->Draw();
            m_Meshes[i]->DrawOutline();
        }
    }

    // --------------------
    // Draw models
    // --------------------
    for (unsigned int i = 0; i < m_Models.size(); i++)
    {
        m_Models[i]->Draw();
    }

    // --------------------
    // Draw transparent meshes
    // --------------------
    using VariantType = std::variant<Mesh*, std::pair<InstanceMesh*, MeshCoordinate>>;
    std::map<float, VariantType> sorted;

    for (unsigned int i = 0; i < m_Transparent_Meshes.size(); i++)
    {
        if (dynamic_cast<InstanceMesh*>(m_Transparent_Meshes[i].get()))
        {
            InstanceMesh* mesh = dynamic_cast<InstanceMesh*>(m_Transparent_Meshes[i].get());
            std::vector<MeshCoordinate>* meshcoord = mesh->GetCoordinates();
            for (const MeshCoordinate coordinate : *meshcoord) {
                float distance = glm::length(GetCurrentCamera()->GetPosition() - coordinate.Position);
                sorted[distance] = std::make_pair(mesh, coordinate);
            }
        }
        else
        {
            float distance = glm::length(GetCurrentCamera()->GetPosition() - m_Transparent_Meshes[i]->GetPosition());
                sorted[distance] = m_Transparent_Meshes[i].get();
        }
    }

    for(std::map<float, VariantType>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) 
    {
        VariantType v = it->second;

        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Mesh*>) {
                Mesh* mesh = dynamic_cast<Mesh*>(arg);
                mesh->Draw(mesh->GetPosition(), mesh->GetScale(), mesh->GetRotation());
                mesh->DrawOutline(mesh->GetPosition(), mesh->GetScale(), mesh->GetRotation());
            } else if constexpr (std::is_same_v<T, std::pair<InstanceMesh*, MeshCoordinate>>) {
                InstanceMesh* mesh = dynamic_cast<std::pair<InstanceMesh*, MeshCoordinate>*>(&arg)->first;
                MeshCoordinate coord = dynamic_cast<std::pair<InstanceMesh*, MeshCoordinate>*>(&arg)->second;
                mesh->Draw(&coord);
                mesh->DrawOutline(&coord);
            }
        }, v);
    }

    // --------------------
    // Draw skybox
    // --------------------
    if (m_Skybox != nullptr)
    {
        GLCall(glDepthFunc(GL_LEQUAL));
        m_Skybox->Draw();
        GLCall(glDepthFunc(GL_LESS));
    }
}

void Scene::Draw(Mesh *mesh)
{
    if (dynamic_cast<InstanceMesh*>(mesh))
    {
        dynamic_cast<InstanceMesh*>(mesh)->Draw();
        dynamic_cast<InstanceMesh*>(mesh)->DrawOutline();
    }
    else
    {
        mesh->Draw(mesh->GetPosition(), mesh->GetScale(), mesh->GetRotation());
        mesh->DrawOutline(mesh->GetPosition(), mesh->GetScale(), mesh->GetRotation());
    }
}
