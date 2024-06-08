#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Mesh.h"
#include "Model.h"

class Scene
{
public:
    Scene() {};
    ~Scene() {};

    static Scene* Create();

    inline static Shader* GetBasicShader() { return s_Instance->m_BasicShader.get(); }
    inline static Shader* GetStandardShader() { return s_Instance->m_StandardShader.get(); }
    inline static Camera* GetCurrentCamera() { return s_Instance->m_CurrentCamera.get(); }
    inline static std::vector<std::shared_ptr<DirectionalLight>>& GetDirectionalLights() { return s_Instance->m_DirectionalLights; }
    inline static std::vector<std::shared_ptr<PointLight>>& GetPointLights() { return s_Instance->m_PointLights; }
    inline static std::vector<std::shared_ptr<SpotLight>>& GetSpotLights() { return s_Instance->m_SpotLights; }

    void Add(std::shared_ptr<Camera> camera);
    void Add(std::shared_ptr<DirectionalLight> light);
    void Add(std::shared_ptr<PointLight> light);
    void Add(std::shared_ptr<SpotLight> light);
    void Add(std::shared_ptr<Mesh> mesh);
    void Add(std::shared_ptr<Model> model);

    void Update();
private:
    static Scene* s_Instance;
    std::shared_ptr<Shader> m_BasicShader;
    std::shared_ptr<Shader> m_StandardShader;
    std::shared_ptr<Camera> m_CurrentCamera;
    std::vector<std::shared_ptr<Camera>> m_Cameras;
    std::vector<std::shared_ptr<DirectionalLight>> m_DirectionalLights;
    std::vector<std::shared_ptr<PointLight>> m_PointLights;
    std::vector<std::shared_ptr<SpotLight>> m_SpotLights;
    std::vector<std::shared_ptr<Mesh>> m_Meshes;
    std::vector<std::shared_ptr<Model>> m_Models;
};