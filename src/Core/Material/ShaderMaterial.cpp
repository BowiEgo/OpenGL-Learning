#include "ShaderMaterial.h"

#include "FileSystem/FileSystem.h"
#include "Core/Scene.h"

ShaderMaterial::ShaderMaterial(Ref<Shader> shader)
{
    m_Shader = shader;
}

ShaderMaterial::~ShaderMaterial()
{
}

void ShaderMaterial::SetDiffuseTexture(std::shared_ptr<Texture2D> texture)
{
    m_DiffuseTexture = texture;
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Material.diffuse", 0);
}

void ShaderMaterial::SetSpecularTexture(std::shared_ptr<Texture2D> texture)
{
    m_SpecularTexture = texture;
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Material.specular", 1);
}

void ShaderMaterial::BindTexture()
{
    if (m_DiffuseTexture != nullptr)
        m_DiffuseTexture->Bind();
        
    if (m_SpecularTexture != nullptr)
        m_SpecularTexture->Bind(1);
}
