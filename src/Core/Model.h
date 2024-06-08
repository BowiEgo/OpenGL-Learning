#pragma once

#include "pch.h"

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

    void Translate(float x, float y, float z);
    inline float* GetTranslate() { return m_Translate; };

    void Draw(Shader* shader);
private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
private:
    ModelOptions m_Options;
    std::vector<std::shared_ptr<Texture2D>> m_Textures_Loaded;
    std::vector<Mesh> m_Meshes;
    std::string m_Directory;
    float m_Translate[3] = { 0.0f, 0.0f, 0.0f };
};