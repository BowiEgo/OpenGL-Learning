#include "ShaderMaterial.h"

#include "FileSystem/FileSystem.h"
#include "Core/Scene.h"

ShaderMaterial::ShaderMaterial(Ref<Shader> shader)
{
    m_Shader = shader;
}

void ShaderMaterial::AddTexture(Ref<Texture2D> texture)
{
    m_Textures.push_back(texture);
}

void ShaderMaterial::SetTextures(std::vector<Ref<Texture2D>> textures)
{
    m_Textures = textures;
}

void ShaderMaterial::BindShader() const
{
    m_Shader->Bind();
}

void ShaderMaterial::UpdateShader(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation) const
{
    // glm::mat4 proj = Scene::GetCurrentCamera()->GetProjMatrix();
    // glm::mat4 view = Scene::GetCurrentCamera()->GetViewMatrix();
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, rotation->first, rotation->second);

    m_Shader->Bind();
    // m_Shader->SetUniform("projectionMatrix", proj);
    // m_Shader->SetUniform("viewMatrix", view);
    m_Shader->SetUniform("modelMatrix", model);

    m_Shader->SetUniform("u_CameraPosition", Scene::GetCurrentCamera()->GetPosition());

    unsigned int diffuseNr = 1;

    for (unsigned int i = 0; i < m_Textures.size(); i++)
    {
        Ref<Texture2D> texture = m_Textures[i];
        texture->Bind(i);
        std::string number;
        std::string type = texture->GetType();
        if(type == "Texture_Diffuse")
            number = std::to_string(diffuseNr++);

        m_Shader->SetUniform(("u_" + type + number).c_str(), i);
    }
}

void ShaderMaterial::UpdateShaderUniform(const std::string &uniformName, const UniformValue &uniformValue) const
{
    m_Shader->Bind();
    m_Shader->SetUniform(uniformName, uniformValue);
}
