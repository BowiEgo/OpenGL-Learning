#include "BasicMaterial.h"

#include "FileSystem/FileSystem.h"
#include "Core/Scene.h"

BasicMaterial::BasicMaterial()
{
    std::string basicVertSrc = FileSystem::ReadFile("../res/shaders/LightCube.vert");
    std::string basicFragSrc = FileSystem::ReadFile("../res/shaders/LightCube.frag");
    m_Shader = std::make_shared<Shader>(basicVertSrc, basicFragSrc, "../res/shaders/LightCube");
    m_Shader->Bind();
    m_Shader->SetUniformVec3("u_Color", { 1.0f, 0.0f, 0.0f });
}

void BasicMaterial::SetColor(const glm::vec3& color)
{
    m_Shader->Bind();
    m_Shader->SetUniformVec3("u_Color", color);
    m_Color = color;
}

void BasicMaterial::BindShader() const
{
    m_Shader->Bind();
}

void BasicMaterial::UpdateShader(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation) const
{
    glm::mat4 proj = Scene::GetCurrentCamera()->GetProjMatrix();
    glm::mat4 view = Scene::GetCurrentCamera()->GetViewMatrix();
    glm::mat4 model(1.0f);
    glm::vec3 modelTranslate(0.0f);
    glm::vec3 modelScale(1.0f);

    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, rotation->first, rotation->second);

    m_Shader->Bind();
    m_Shader->SetUniformMat4("projectionMatrix", proj);
    m_Shader->SetUniformMat4("viewMatrix",       view);
    m_Shader->SetUniformMat4("modelMatrix",      model);
}

void BasicMaterial::UpdateShaderUniform(const std::string &uniformName, const UniformValue &uniformValue) const
{
    m_Shader->Bind();
    m_Shader->SetUniform(uniformName, uniformValue);
}