#pragma once

#include "pch.h"
#include "Camera.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Mesh.h"
#include "InstanceMesh.h"
#include "Model.h"
#include "Material/MaterialManager.h"

class Scene
{
public:
    Scene();
    ~Scene();

    static Scene* Create();

    inline static MaterialManager* GetMaterialManager() { return s_MaterialManager; }
    inline static Texture2D* GetVoidTexture2D() { return s_VoidTexture2D; }
    inline static TextureCubemap* GetVoidTextureCubemap() { return s_VoidTextureCubemap; }
    inline static Camera* GetCurrentCamera() { return s_Instance->m_CurrentCamera.get(); }
    inline static std::vector<Ref<Camera>>* GetCameras() { return &(s_Instance->m_Cameras); }
    inline static std::vector<Ref<DirectionalLight>>& GetDirectionalLights() { return s_Instance->m_DirectionalLights; }
    inline static std::vector<Ref<PointLight>>& GetPointLights() { return s_Instance->m_PointLights; }
    inline static std::vector<Ref<SpotLight>>& GetSpotLights() { return s_Instance->m_SpotLights; }
    inline static Ref<Mesh>& GetSkybox() { return s_Instance->m_Skybox; }

    void SetSkybox(Ref<Mesh> skybox);
    void Add(Ref<Camera> camera);
    void Add(Ref<DirectionalLight> light);
    void Add(Ref<PointLight> light);
    void Add(Ref<SpotLight> light);
    void Add(Ref<Mesh> mesh);
    void Add(Ref<InstanceMesh> mesh);
    void Add(Ref<Model> model);
    void SetCurrentCamera(Ref<Camera> camera);

    void Draw();
    void Draw(Mesh* mesh);
private:
    static Scene* s_Instance;
    static MaterialManager* s_MaterialManager;
    static Texture2D* s_VoidTexture2D;
    static TextureCubemap* s_VoidTextureCubemap;
    Ref<Camera> m_CurrentCamera;
    Ref<Mesh> m_Skybox;
    std::vector<Ref<Camera>> m_Cameras;
    std::vector<Ref<DirectionalLight>> m_DirectionalLights;
    std::vector<Ref<PointLight>> m_PointLights;
    std::vector<Ref<SpotLight>> m_SpotLights;
    std::vector<Ref<Mesh>> m_Meshes;
    std::vector<Ref<Mesh>> m_Transparent_Meshes;
    std::vector<Ref<Model>> m_Models;
};