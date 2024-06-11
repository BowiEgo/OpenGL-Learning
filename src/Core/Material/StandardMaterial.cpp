#include "StandardMaterial.h"

#include "Core/Scene.h"
#include "FileSystem/FileSystem.h"

StandardMaterial::StandardMaterial()
{
    std::string standardVertSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.vert");
    std::string standardFragSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.frag");
    m_Shader = std::make_shared<Shader>(standardVertSrc, standardFragSrc);

    m_Shader->Bind();
    m_Shader->SetUniform1f("u_Material.shininess", m_MaterialShininess);
}

void StandardMaterial::SetDiffuseTexture(Ref<Texture2D> texture)
{
    m_DiffuseTexture = texture;
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Material.diffuse", 0);
}

void StandardMaterial::SetSpecularTexture(Ref<Texture2D> texture)
{
    m_SpecularTexture = texture;
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Material.specular", 1);
}

void StandardMaterial::BindTexture() const
{
    if (m_DiffuseTexture != nullptr)
        m_DiffuseTexture->Bind();
        
    if (m_SpecularTexture != nullptr)
        m_SpecularTexture->Bind(1);
}

void StandardMaterial::BindShader() const
{
    m_Shader->Bind();
}

void StandardMaterial::UpdateShader(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation) const
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
    m_Shader->SetUniform("u_Material.shininess", m_MaterialShininess);
    m_Shader->SetUniform("u_Is_Opaque", Is_Opaque);
    m_Shader->SetUniform("u_Discard_Transparent", Discard_Transparent);

    for (unsigned int i = 0; i < Scene::GetDirectionalLights().size(); i++)
    {
        Scene::GetDirectionalLights()[i]->Update(m_Shader.get());
    }

    for (unsigned int i = 0; i < Scene::GetPointLights().size(); i++)
    {
        m_Shader->SetUniform("u_NR_PointLights", (int)Scene::GetPointLights().size());
        Scene::GetPointLights()[i]->Update(m_Shader.get());
    }

    for (unsigned int i = 0; i < Scene::GetSpotLights().size(); i++)
    {
        Scene::GetSpotLights()[i]->Update(m_Shader.get());
    }

    BindTexture();
}

void StandardMaterial::UpdateShaderUniform(const std::string &uniformName, const UniformValue &uniformValue) const
{
    m_Shader->Bind();
    m_Shader->SetUniform(uniformName, uniformValue);
}
