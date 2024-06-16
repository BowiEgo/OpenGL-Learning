#pragma once

#include "pch.h"

#include "Material/StandardMaterial.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

struct ModelOptions {
    bool FlipUVs = false;
};

class Model
{
public:
    Model(const std::string& path, const ModelOptions& options = ModelOptions());
    ~Model();

    void Translate(float x, float y, float z);
    void Scale(float x, float y, float z);
    void Rotate(std::pair<float, glm::vec3>& rotation);

    inline glm::vec3& GetTranslate() { return m_Translate; }
    inline glm::vec3& GetScale() { return m_Scale; }
    inline std::pair<float, glm::vec3>* GetRotation() { return &m_Rotation; }

    inline std::vector<Ref<Mesh>>& GetMeshes() { return m_Meshes; }
    inline Ref<aiAABB> GetAABB() { return m_AABB; }
    inline glm::vec3 GetCenter() { return glm::vec3(
        (m_AABB->mMax.x - m_AABB->mMin.x) / 1.0f,
        (m_AABB->mMax.y - m_AABB->mMin.y) / 1.0f,
        (m_AABB->mMax.z - m_AABB->mMin.z) / 1.0f
    ); }

    void SetOutlineWidth(float& width);
    void SetOutlineColor(glm::vec3& color);

    void Draw();
    void DrawOutline();
private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Ref<Mesh> ProcessMesh(aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName, Ref<StandardMaterial> targetMaterial);
public:
    bool Outline_Enabled = false;
    bool Outline_SingleMesh = false;
private:
    static std::vector<Ref<Texture2D>> s_Textures_Loaded;

    ModelOptions m_Options;

    std::vector<Ref<Mesh>> m_Meshes;
    std::string m_Directory;

    glm::vec3 m_Translate = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    std::pair<float, glm::vec3> m_Rotation = { 0.0, glm::vec3(0.0, 0.0, 1.0) };

    Ref<aiAABB> m_AABB = std::make_shared<aiAABB>();

    float m_Outline_Width = 1.0f;
    glm::vec3 m_Outline_Color = glm::vec3(0.7647f, 0.9294f, 0.9137f);
};