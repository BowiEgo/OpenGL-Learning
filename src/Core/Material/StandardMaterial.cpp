#include "StandardMaterial.h"
#include "CubemapMaterial.h"

#include "Core/Scene.h"
#include "FileSystem/FileSystem.h"

StandardMaterial::StandardMaterial()
{
    std::string standardVertSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.vert");
    std::string standardFragSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.frag");
    m_Shader = std::make_shared<Shader>(standardVertSrc, standardFragSrc, "../res/shaders/ModelLoading");

    m_Shader->Bind();
    m_Shader->SetUniform("u_Material.shininess", m_MaterialShininess);
}

void StandardMaterial::AddTexture(Ref<Texture2D> texture)
{
    if (texture->GetType().find("Diffuse") != std::string::npos)
    {
        m_Diffuse_Textures.push_back(texture);
    }
    else if (texture->GetType().find("Specular") != std::string::npos)
    {
        m_Specular_Textures.push_back(texture);
    }
    else if (texture->GetType().find("Normal") != std::string::npos)
    {
        m_Normal_Textures.push_back(texture);
    }
    else if (texture->GetType().find("Height") != std::string::npos)
    {
        m_Height_Textures.push_back(texture);
    }
}

void StandardMaterial::SetTextures(std::vector<Ref<Texture2D>> textures)
{
    for (auto texture : textures)
    {
        AddTexture(texture);
    }
}

void StandardMaterial::SetEnvironmentMixRate(const float &rate)
{
    m_Environment_Mix_Rate = rate;
}

void StandardMaterial::SetEnvironmentRefractIndex(const float &index)
{
    m_Environment_Refractive_Index = index;
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

    m_Shader->SetUniform("u_Is_EnvironmentTexture_Valid", Environment_Enabled);
    m_Shader->SetUniform("u_Texture_Env_Mix_Rate", m_Environment_Mix_Rate);
    m_Shader->SetUniform("u_Texture_Env_Refractive_Index", m_Environment_Refractive_Index);

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

    BindTextures();
}

void StandardMaterial::UpdateShaderUniform(const std::string &uniformName, const UniformValue &uniformValue) const
{
    m_Shader->Bind();
    m_Shader->SetUniform(uniformName, uniformValue);
}

void StandardMaterial::BindTextures() const
{
    unsigned int diffuseNr = m_Diffuse_Textures.size();
    unsigned int specularNr = m_Specular_Textures.size();
    unsigned int normalNr = m_Normal_Textures.size();
    unsigned int heightNr = m_Height_Textures.size();

    unsigned int slot;

    if (diffuseNr == 0)
    {
        slot = 0;
        Scene::GetVoidTexture2D()->Bind(slot);
        m_Shader->SetUniform("u_Texture_Diffuse1", slot);
        diffuseNr = 1;
    }
    else
    {
        for (unsigned int i = 0; i < diffuseNr; i++)
        {
            slot = i;
            m_Diffuse_Textures[i]->Bind(slot);
            m_Shader->SetUniform("u_Texture_Diffuse" + std::to_string(i + 1), slot);
        }
    }

    slot = diffuseNr;
    if (specularNr == 0)
    {
        // Scene::GetVoidTexture2D()->Bind(slot);
        // m_Shader->SetUniform(("u_Texture_Specular1"), slot);
        // specularNr = 1;
    }
    else
    {
        for (unsigned int i = 0; i < specularNr; i++)
        {
           
            slot += i;
            m_Specular_Textures[i]->Bind(slot);
            m_Shader->SetUniform("u_Texture_Specular" + std::to_string(i + 1), slot);
        }
    }

    slot = diffuseNr + specularNr;
    if (normalNr == 0)
    {
        // Scene::GetVoidTexture2D()->Bind(slot);
        // m_Shader->SetUniform(("u_Texture_Normal1"), slot);
        // normalNr = 1;
    }
    else
    {
        for (unsigned int i = 0; i < normalNr; i++)
        {
           
            slot += i;
            m_Normal_Textures[i]->Bind(slot);
            m_Shader->SetUniform("u_Texture_Normal" + std::to_string(i + 1), slot);
        }
    }

    slot = diffuseNr + specularNr + normalNr;
    if (heightNr == 0)
    {
        // Scene::GetVoidTexture2D()->Bind(slot);
        // m_Shader->SetUniform(("u_Texture_Height1"), slot);
        // heightNr = 1;
    }
    else
    {
        for (unsigned int i = 0; i < heightNr; i++)
        {
            slot += i;
            m_Height_Textures[i]->Bind(slot);
            m_Shader->SetUniform("u_Texture_Height" + std::to_string(i + 1), slot);
        }
    }

    unsigned int allNr = diffuseNr + specularNr + normalNr + heightNr;
    if (Scene::GetSkybox() != nullptr)
    {
        dynamic_cast<CubemapMaterial*>(Scene::GetSkybox()->GetMaterial().get())->BindTexture(allNr);
        m_Shader->SetUniform("u_Texture_Environment", allNr);
    } else {
        Scene::GetVoidTextureCubemap()->Bind(allNr);
        m_Shader->SetUniform("u_Texture_Environment", allNr);
    }
}
