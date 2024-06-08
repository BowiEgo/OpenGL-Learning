#include "StandardMaterial.h"

#include "Core/Scene.h"

StandardMaterial::StandardMaterial()
{
    Scene::GetStandardShader()->Bind();
    Scene::GetStandardShader()->SetUniform1f("u_Material.shininess", m_MaterialShininess);
}

StandardMaterial::~StandardMaterial()
{
}

void StandardMaterial::SetDiffuseTexture(std::shared_ptr<Texture2D> texture)
{
    m_DiffuseTexture = texture;
    Scene::GetStandardShader()->Bind();
    Scene::GetStandardShader()->SetUniform1i("u_Material.diffuse", 0);
}

void StandardMaterial::SetSpecularTexture(std::shared_ptr<Texture2D> texture)
{
    m_SpecularTexture = texture;
    Scene::GetStandardShader()->Bind();
    Scene::GetStandardShader()->SetUniform1i("u_Material.specular", 1);
}

void StandardMaterial::BindTexture()
{
    m_DiffuseTexture->Bind();
    m_SpecularTexture->Bind(1);
}
