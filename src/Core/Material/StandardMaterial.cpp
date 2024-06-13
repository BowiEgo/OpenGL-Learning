#include "StandardMaterial.h"
#include "CubemapMaterial.h"

#include "Core/Scene.h"
#include "FileSystem/FileSystem.h"

StandardMaterial::StandardMaterial()
{
    std::string standardVertSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.vert");
    std::string standardFragSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.frag");
    m_Shader = std::make_shared<Shader>(standardVertSrc, standardFragSrc);

    m_Shader->Bind();
    m_Shader->SetUniform("u_Material.shininess", m_MaterialShininess);

    SetEnvironmentTexture(TextureCubemap::CreateEmptyTexture("Texture_Environment"));
    m_Is_Environment_Valid = false;

    // if (Scene::GetSkybox() != nullptr)
    // {
        // dynamic_cast<CubemapMaterial*>(Scene::GetSkybox()->GetMaterial().get())->BindTexture();
    // m_Shader->SetUniform("u_EnvironmentTexture", 2);
    // }
}

void StandardMaterial::AddTexture(Ref<Texture2D> texture)
{
    m_Textures.push_back(texture);
}

void StandardMaterial::SetTextures(std::vector<Ref<Texture2D>> textures)
{
    m_Textures = textures;
}

void StandardMaterial::SetEnvironmentTexture(Ref<TextureCubemap> texture)
{
    m_Env_Texture = texture;
    m_Is_Environment_Valid = true;
}

void StandardMaterial::SetEnvironmentMixRate(const float &rate)
{
    m_Environment_Mix_Rate = rate;
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

    m_Shader->SetUniform("u_Is_EnvironmentTexture_Valid", m_Is_Environment_Valid);
    m_Shader->SetUniform("u_Texture_Env_Mix_Rate", m_Environment_Mix_Rate);

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
    unsigned int size = m_Textures.size();
    
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < size; i++)
    {
        Ref<Texture2D> texture = m_Textures[i];
        texture->Bind(i);

        std::string number;
        std::string type = texture->GetType();

        if(type == "Texture_Diffuse")
            number = std::to_string(diffuseNr++);
        else if(type == "Texture_Specular")
            number = std::to_string(specularNr++);
        else if(type == "Texture_Normal")
            number = std::to_string(normalNr++);
        else if(type == "Texture_Height")
            number = std::to_string(heightNr++);

        m_Shader->SetUniform(("u_" + type + number).c_str(), i);
    }

    m_Env_Texture->Bind(size);
    m_Shader->SetUniform("u_Texture_Environment", size);
}
