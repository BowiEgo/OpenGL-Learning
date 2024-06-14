#include "Model.h"

#include "Scene.h"

Model::Model(const std::string &path, const ModelOptions &options)
    : m_Options(options)
{
    LoadModel(path);
}

Model::~Model()
{
}

void Model::Translate(float x, float y, float z)
{
    m_Translate.x = x;
    m_Translate.y = y;
    m_Translate.z = z;
}

void Model::Scale(float x, float y, float z)
{
    m_Scale.x = x;
    m_Scale.y = y;
    m_Scale.z = z;
}

void Model::SetOutlineWidth(float &width)
{
    m_Outline_Width = width;
}

void Model::SetOutlineColor(glm::vec3 &color)
{
    m_Outline_Color = color;
}

void Model::Draw()
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {

        m_Meshes[i]->Draw(GetTranslate(), GetScale(), GetRotation());

        if (Outline_Enabled && Outline_SingleMesh && m_Meshes[i]->Outline_Enabled)
        {
            m_Meshes[i]->SetOutlineWidth(m_Outline_Width);
            m_Meshes[i]->SetOutlineColor(m_Outline_Color);
            m_Meshes[i]->DrawOutline(GetTranslate(), GetScale(), GetRotation());
        }
    }

    if (Outline_Enabled && !Outline_SingleMesh)
    {
        DrawOutline();
    }
}

void Model::DrawOutline()
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        m_Meshes[i]->SetOutlineWidth(m_Outline_Width);
        m_Meshes[i]->SetOutlineColor(m_Outline_Color);
        m_Meshes[i]->DrawOutline(GetTranslate(), GetScale(), GetRotation());
    }
}

void Model::LoadModel(const std::string &path)
{
    Assimp::Importer importer;
    unsigned int flags = aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_GenBoundingBoxes;
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

Ref<Mesh> Model::ProcessMesh(aiMesh *aimesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Ref<Texture2D>> textures;

    const aiAABB &aabb = aimesh->mAABB;
    if (aabb.mMin.x < m_AABB->mMin.x)
        m_AABB->mMin.x = aabb.mMin.x;
    if (aabb.mMax.x > m_AABB->mMax.x)
        m_AABB->mMax.x = aabb.mMax.x;

    if (aabb.mMin.y < m_AABB->mMin.y)
        m_AABB->mMin.y = aabb.mMin.y;
    if (aabb.mMax.y > m_AABB->mMax.y)
        m_AABB->mMax.y = aabb.mMax.y;

    if (aabb.mMin.z < m_AABB->mMin.z)
        m_AABB->mMin.z = aabb.mMin.z;
    if (aabb.mMax.z > m_AABB->mMax.z)
        m_AABB->mMax.z = aabb.mMax.z;

    for(unsigned int i = 0; i < aimesh->mNumVertices; i++)
    {
        Vertex vertex;
        // Handle vertex position
        glm::vec3 vector;
        vector.x = aimesh->mVertices[i].x;
        vector.y = aimesh->mVertices[i].y;
        vector.z = aimesh->mVertices[i].z; 
        vertex.Position = vector;
        // Handle vertex normal
        vector.x = aimesh->mNormals[i].x;
        vector.y = aimesh->mNormals[i].y;
        vector.z = aimesh->mNormals[i].z;
        vertex.Normal = vector;
        // Handle vertex texture
        if(aimesh->mTextureCoords[0]) // Check if aimesh has textureCoords
        {
            glm::vec2 vec;
            vec.x = aimesh->mTextureCoords[0][i].x; 
            vec.y = aimesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // // tangent
            vector.x = aimesh->mTangents[i].x;
            vector.y = aimesh->mTangents[i].y;
            vector.z = aimesh->mTangents[i].z;
            vertex.Tangent = vector;
            // // bitangent
            vector.x = aimesh->mBitangents[i].x;
            vector.y = aimesh->mBitangents[i].y;
            vector.z = aimesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // Handle indices
    for(unsigned int i = 0; i < aimesh->mNumFaces; i++)
    {
        aiFace face = aimesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // Handle materials
    Ref<StandardMaterial> mMaterial = std::make_shared<StandardMaterial>();
    if(aimesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[aimesh->mMaterialIndex];
        loadMaterialTextures(material, aiTextureType_DIFFUSE, "Texture_Diffuse", mMaterial);
        loadMaterialTextures(material, aiTextureType_SPECULAR, "Texture_Specular", mMaterial);
        loadMaterialTextures(material, aiTextureType_HEIGHT, "Texture_Normal", mMaterial);
        loadMaterialTextures(material, aiTextureType_AMBIENT, "Texture_Height", mMaterial);
    }

    Ref<Mesh> mesh = std::make_shared<Mesh>(vertices, indices, mMaterial);
    mesh->Outline_Enabled = true;
    mesh->Outline_DrawType = OUTLINE_DRAWTYPE_NORMAL;
    return mesh;
}

void Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName, Ref<StandardMaterial> targetMaterial)
{
    std::vector<Ref<Texture2D>> textures;
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
                targetMaterial->AddTexture(m_Textures_Loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {
            Ref<Texture2D> texture = std::make_unique<Texture2D>(typeName, filePath, texOpts);

            targetMaterial->AddTexture(texture);
            m_Textures_Loaded.push_back(texture); // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
}
