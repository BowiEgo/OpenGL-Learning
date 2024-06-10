#include "ShaderMaterial.h"

#include "FileSystem/FileSystem.h"
#include "Core/Scene.h"

ShaderMaterial::ShaderMaterial(Ref<Shader> shader)
{
    m_Shader = shader;
}

void ShaderMaterial::SetDiffuseTexture(Ref<Texture2D> texture)
{
    m_DiffuseTexture = texture;
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Material.diffuse", 0);
}

void ShaderMaterial::SetSpecularTexture(Ref<Texture2D> texture)
{
    m_SpecularTexture = texture;
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Material.specular", 1);
}

void ShaderMaterial::BindTexture() const
{
    if (m_DiffuseTexture != nullptr)
        m_DiffuseTexture->Bind();
        
    if (m_SpecularTexture != nullptr)
        m_SpecularTexture->Bind(1);
}

void ShaderMaterial::BindShader() const
{
    m_Shader->Bind();
}

void ShaderMaterial::UpdateShader(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation) const
{
    glm::mat4 proj = Scene::GetCurrentCamera()->GetProjMatrix();
    glm::mat4 view = Scene::GetCurrentCamera()->GetViewMatrix();
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, rotation->first, rotation->second);

    m_Shader->Bind();
    m_Shader->SetUniform("projectionMatrix", proj);
    m_Shader->SetUniform("viewMatrix", view);
    m_Shader->SetUniform("modelMatrix", model);

    m_Shader->SetUniform("u_CameraPosition", Scene::GetCurrentCamera()->GetPosition());

    BindTexture();
}

void ShaderMaterial::UpdateShaderUniform(const std::string &uniformName, const UniformValue &uniformValue) const
{
    m_Shader->Bind();
    m_Shader->SetUniform(uniformName, uniformValue);
}
