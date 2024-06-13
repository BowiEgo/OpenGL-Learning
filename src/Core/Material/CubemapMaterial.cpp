#include "CubemapMaterial.h"

#include "Core/Scene.h"
#include "FileSystem/FileSystem.h"

CubemapMaterial::CubemapMaterial()
{
    std::string standardVertSrc = FileSystem::ReadFile("../res/shaders/Cubemap.vert");
    std::string standardFragSrc = FileSystem::ReadFile("../res/shaders/Cubemap.frag");
    m_Shader = std::make_shared<Shader>(standardVertSrc, standardFragSrc);

    m_Shader->Bind();
}

void CubemapMaterial::SetCubemapTexture(Ref<TextureCubemap> texture)
{
    m_CubemapTexture = texture;
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_CubemapTexture", 0);
}

void CubemapMaterial::BindTexture() const
{
    if (m_CubemapTexture != nullptr)
        m_CubemapTexture->Bind();
}

void CubemapMaterial::BindShader() const
{
    m_Shader->Bind();
}

void CubemapMaterial::UpdateShader(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation) const
{
    glm::mat4 proj = Scene::GetCurrentCamera()->GetProjMatrix();
    glm::mat4 view = glm::mat3(Scene::GetCurrentCamera()->GetViewMatrix());

    m_Shader->Bind();
    m_Shader->SetUniform("projectionMatrix", proj);
    m_Shader->SetUniform("viewMatrix", view);

    BindTexture();
}

void CubemapMaterial::UpdateShaderUniform(const std::string &uniformName, const UniformValue &uniformValue) const
{
    m_Shader->Bind();
    m_Shader->SetUniform(uniformName, uniformValue);
}
