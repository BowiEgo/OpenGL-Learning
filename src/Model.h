#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(const std::string& path);

    void Draw(Shader* shader);
private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
private:
    std::vector<Mesh> m_Meshes;
    std::string m_Directory;
};