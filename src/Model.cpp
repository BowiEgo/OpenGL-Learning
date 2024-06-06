#include "Model.h"

Model::Model(const std::string &path)
{
    LoadModel(path);
}

void Model::Draw(Shader *shader)
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
        m_Meshes[i].Draw(shader);
}

void Model::LoadModel(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        CORE_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
        return;
    }

    m_Directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture2D>> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // Handle vertex position
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z; 
        vertex.Position = vector;
        // Handle vertex normal
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // Handle vertex texture
        if(mesh->mTextureCoords[0]) // Check if mesh has textureCoords
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // Handle indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // Handle materials
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        for (auto& tex : diffuseMaps)
        {
            textures.push_back(tex);
        }
        auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        for (auto& tex : specularMaps)
        {
            textures.push_back(tex);
        }
    }

    return Mesh(vertices, indices, textures);
}

std::vector<std::shared_ptr<Texture2D>> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
{
    std::vector<std::shared_ptr<Texture2D>> textures;
    for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString path;
        material->GetTexture(type, i, &path);
        std::shared_ptr<Texture2D> texture = std::make_unique<Texture2D>(typeName, m_Directory + "/" + path.C_Str());
        textures.push_back(texture);
    }
    return textures;
}
