#include "Model.h"

#include "Material/StandardMaterial.h"

Model::Model(const std::string &path, const ModelOptions &options)
    : m_Options(options)
{
    LoadModel(path);
}

void Model::Translate(float x, float y, float z)
{
    m_Translate[0] = x;
    m_Translate[1] = y;
    m_Translate[2] = z;
}

void Model::Draw(Shader* shader)
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
        m_Meshes[i].Draw(shader);
}

void Model::LoadModel(const std::string &path)
{
    Assimp::Importer importer;
    unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace;
    if (m_Options.FlipUVs)
        flags |= aiProcess_FlipUVs;

    const aiScene* scene = importer.ReadFile(path, flags);

    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
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
            // // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
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
        auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "Texture_Diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "Texture_Specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        auto normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "Texture_Normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        auto heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "Texture_Height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    return Mesh(vertices, indices, textures, std::make_shared<StandardMaterial>());
}

std::vector<std::shared_ptr<Texture2D>> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
{
    std::vector<std::shared_ptr<Texture2D>> textures;
    TextureOptions texOpts;
    texOpts.wrapS = GL_REPEAT;
    texOpts.wrapT = GL_REPEAT;
    texOpts.minFilter = GL_LINEAR_MIPMAP_LINEAR;
    texOpts.magFilter = GL_LINEAR;

    for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString path;
        material->GetTexture(type, i, &path);
        std::string filePath = m_Directory + "/" + path.C_Str();
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < m_Textures_Loaded.size(); j++)
        {
            if(std::strcmp(m_Textures_Loaded[j]->GetFilePath().data(), filePath.c_str()) == 0)
            {
                textures.push_back(m_Textures_Loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {
            std::shared_ptr<Texture2D> texture = std::make_unique<Texture2D>(typeName, filePath, texOpts);

            textures.push_back(texture);
            m_Textures_Loaded.push_back(texture); // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}
