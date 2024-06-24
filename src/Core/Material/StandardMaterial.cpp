#include "StandardMaterial.h"
#include "CubemapMaterial.h"

#include "Core/Scene.h"
#include "FileSystem/FileSystem.h"

std::string StandardMaterial::s_VertSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.vert");
std::string StandardMaterial::s_FragSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.frag");

StandardMaterial::StandardMaterial()
{
    m_VertSrc = s_VertSrc;
    m_FragSrc = s_FragSrc;
    Setup();
}

void StandardMaterial::Setup()
{
    if (m_GeomSrc.empty()) {
        m_Shader = std::make_shared<Shader>(m_VertSrc, m_FragSrc);
    } else {
        m_Shader = std::make_shared<Shader>(m_VertSrc, m_GeomSrc, m_FragSrc);
    }

    m_Shader->Bind();
    m_Shader->SetUniform("u_Material.shininess", m_MaterialShininess);
}

void StandardMaterial::BindVertexShader(const std::string &filepath)
{
    std::string vertSrc = FileSystem::ReadFile(filepath);
    if (vertSrc.empty()) {
        throw std::runtime_error("Failed to load vertex shader source.");
    } else {
        m_VertSrc = vertSrc;
    }
}

void StandardMaterial::BindFragmentShader(const std::string &filepath)
{
    std::string fragSrc = FileSystem::ReadFile(filepath);
    if (fragSrc.empty()) {
        throw std::runtime_error("Failed to load fragment shader source.");
    } else {
        m_FragSrc = fragSrc;
    }
}

void StandardMaterial::BindGeometryShader(const std::string& filepath)
{
    std::string geomSrc = FileSystem::ReadFile(filepath);
    if (geomSrc.empty()) {
        throw std::runtime_error("Failed to load geometry shader source.");
    } else {
        m_GeomSrc = geomSrc;
    }
}

void StandardMaterial::AddTexture(Ref<Texture> texture)
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
    else if (texture->GetType().find("ShadowMap") != std::string::npos)
    {
        if (dynamic_cast<Texture2D*>(texture.get()))
            m_ShadowMap_Textures.push_back(texture);
        else if (dynamic_cast<TextureCubemap*>(texture.get()))
            m_CubeShadowMap_Textures.push_back(texture);
    }
}

void StandardMaterial::SetTextures(std::vector<Ref<Texture2D>> textures)
{
    for (auto texture : textures)
    {
        AddTexture(texture);
    }
}

void StandardMaterial::SetEnvironmentReflectiveRate(const float &rate)
{
    m_Environment_Reflective_Rate = rate;
}

void StandardMaterial::SetEnvironmentRefractiveRate(const float &rate)
{
    m_Environment_Refractive_Rate = rate;
}

void StandardMaterial::SetEnvironmentRefractiveIndex(const float &index)
{
    m_Environment_Refractive_Index = index;
}

void StandardMaterial::BindShader() const
{
    m_Shader->Bind();
}

void StandardMaterial::UpdateShader(const glm::vec3& position, const glm::vec3& scale, const std::pair<float, glm::vec3>* rotation) const
{
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, rotation->first, rotation->second);

    m_Shader->Bind();
    m_Shader->SetUniform("modelMatrix", model);

    m_Shader->SetUniform("u_CameraPosition", Scene::GetCurrentCamera()->GetPosition());
    m_Shader->SetUniform("u_Material.shininess", m_MaterialShininess);
    m_Shader->SetUniform("u_Is_Opaque", Is_Opaque);
    m_Shader->SetUniform("u_Discard_Transparent", Discard_Transparent);

    m_Shader->SetUniform("u_Is_EnvironmentTexture_Valid", Environment_Enabled);
    m_Shader->SetUniform("u_Env_Reflective_Rate", m_Environment_Reflective_Rate);
    m_Shader->SetUniform("u_Env_Refractive_Rate", m_Environment_Refractive_Rate);
    m_Shader->SetUniform("u_Env_Refractive_Index", m_Environment_Refractive_Index);

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
    unsigned int shadowMapNr = m_ShadowMap_Textures.size();
    unsigned int cubeShadowMapNr = m_CubeShadowMap_Textures.size();

    unsigned int slot = 0;

    if (diffuseNr == 0)
    {
        Scene::GetVoidTexture2D()->Bind(slot);
        m_Shader->SetUniform("u_Texture_Diffuse1", slot);
        diffuseNr = 1;
    }
    else
    {
        for (unsigned int i = 0; i < diffuseNr; i++)
        {
            m_Diffuse_Textures[i]->Bind(slot);
            m_Shader->SetUniform("u_Texture_Diffuse" + std::to_string(i + 1), slot);
            slot++;
        }
    }

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
           
            m_Specular_Textures[i]->Bind(slot);
            m_Shader->SetUniform("u_Texture_Specular" + std::to_string(i + 1), slot);
            slot++;
        }
    }

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
           
            m_Normal_Textures[i]->Bind(slot);
            m_Shader->SetUniform("u_Texture_Normal" + std::to_string(i + 1), slot);
            slot++;
        }
    }

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
            slot++;
        }
    }

    if (shadowMapNr == 0)
    {
        // Scene::GetVoidTexture2D()->Bind(slot);
        // m_Shader->SetUniform(("u_Texture_Height1"), slot);
        // shadowMapNr = 1;
        Scene::GetVoidTextureCubemap()->Bind(slot);
        m_Shader->SetUniform("u_Texture_ShadowMap1", slot);
        slot++;
    }
    else
    {
        for (unsigned int i = 0; i < shadowMapNr; i++)
        {
            slot += i;
            m_ShadowMap_Textures[i]->Bind(slot);
            m_Shader->SetUniform("u_Texture_ShadowMap" + std::to_string(i + 1), slot);
            slot++;
        }
    }

    if (cubeShadowMapNr == 0)
    {
        // Scene::GetVoidTexture2D()->Bind(slot);
        // m_Shader->SetUniform(("u_Texture_Height1"), slot);
        // cubeShadowMapNr = 1;
        Scene::GetVoidTextureCubemap()->Bind(slot);
        m_Shader->SetUniform("u_Texture_CubeShadowMap1", slot);
        slot++;
    }
    else
    {
        for (unsigned int i = 0; i < cubeShadowMapNr; i++)
        {
            m_CubeShadowMap_Textures[i]->Bind(slot);
            m_Shader->SetUniform("u_Texture_CubeShadowMap" + std::to_string(i + 1), slot);
            slot++;
        }
    }

    if (Scene::GetSkybox() != nullptr)
    {
        dynamic_cast<CubemapMaterial*>(Scene::GetSkybox()->GetMaterial().get())->BindTexture(slot);
        m_Shader->SetUniform("u_Texture_Environment", slot);
    } else {
        Scene::GetVoidTextureCubemap()->Bind(slot);
        m_Shader->SetUniform("u_Texture_Environment", slot);
    }
}
