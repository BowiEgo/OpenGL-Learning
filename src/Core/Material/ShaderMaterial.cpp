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

void ShaderMaterial::AddTexture(const std::string &name, const Ref<Texture2D> texture)
{
    m_Named_Textures.push_back({ name, texture });
}

void ShaderMaterial::AddTexture(const std::string &name, const Ref<TextureCubemap> texture)
{
    m_Named_CubemapTextures.push_back({ name, texture });
}

void ShaderMaterial::SetTextures(std::vector<Ref<Texture2D>> textures)
{
    m_Textures = textures;
}

void ShaderMaterial::BindShader() const
{
    m_Shader->Bind();
}

void ShaderMaterial::UpdateShader(const glm::vec3& position, const glm::vec3& scale, const std::pair<float, glm::vec3>* rotation) const
{
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, rotation->first, rotation->second);

    m_Shader->Bind();
    m_Shader->SetUniform("modelMatrix", model);

    m_Shader->SetUniform("u_CameraPosition", Scene::GetCurrentCamera()->GetPosition());

    for (unsigned int i = 0; i < Scene::GetPointLights().size(); i++)
    {
        m_Shader->SetUniform("u_NR_PointLights", (int)Scene::GetPointLights().size());
        Scene::GetPointLights()[i]->Update(m_Shader.get());
    }

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

    for (unsigned int i = 0; i < m_Named_Textures.size(); i++)
    {
        std::string name = m_Named_Textures[i].first;
        Ref<Texture2D> texture = m_Named_Textures[i].second;

        unsigned int slot = m_Textures.size() + i;
        texture->Bind(slot);
        m_Shader->SetUniform((name).c_str(), slot);
    }

    unsigned int irradianceMapNr = 0;
    for (unsigned int i = 0; i < m_Named_CubemapTextures.size(); i++)
    {
        std::string name = m_Named_CubemapTextures[i].first;
        Ref<TextureCubemap> texture = m_Named_CubemapTextures[i].second;

        unsigned int slot = m_Textures.size() + m_Named_Textures.size() + i + 1;
        texture->Bind(slot);
        m_Shader->SetUniform((name).c_str(), slot);
        if (name == "irradianceMap")
        {
            irradianceMapNr++;
        }
    }
    // if (irradianceMapNr == 0)
    // {
    //     unsigned int slot = m_Textures.size() + m_Named_Textures.size() + m_Named_CubemapTextures.size();
    //     Scene::GetVoidTextureCubemap()->Bind(slot);
    //     m_Shader->SetUniform("irradianceMap", slot);
    // }
}

void ShaderMaterial::UpdateShaderUniform(const std::string &uniformName, const UniformValue &uniformValue) const
{
    m_Shader->Bind();
    m_Shader->SetUniform(uniformName, uniformValue);
}
