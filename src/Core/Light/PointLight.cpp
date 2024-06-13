#include "PointLight.h"

PointLight::PointLight(float position[3])
{
    m_Position[0] = position[0];
    m_Position[1] = position[1];
    m_Position[2] = position[2];
}

PointLight::PointLight(const glm::vec3 &position)
{
    m_Position[0] = position.x;
    m_Position[1] = position.y;
    m_Position[2] = position.z;
}

void PointLight::SetEnabled(bool& value)
{
    m_IsEnabled = value;
}

void PointLight::SetPositions(float position[3])
{
    m_Position[0] = position[0];
    m_Position[1] = position[1];
    m_Position[2] = position[2];
}

void PointLight::SetPositions(glm::vec3 &position)
{
    m_Position[0] = position.x;
    m_Position[1] = position.y;
    m_Position[2] = position.z;
}

void PointLight::SetID(unsigned int id)
{
    m_RendererID = id;
}

void PointLight::Update(Shader* shader)
{
    std::string uniformBase = "u_PointLights[" + std::to_string(m_RendererID) + "].";

    shader->Bind();
    shader->SetUniformBool((uniformBase + "enable").c_str(),    m_IsEnabled);
    shader->SetUniformVec3((uniformBase + "ambient").c_str(),   m_Ambient);
    shader->SetUniformVec3((uniformBase + "diffuse").c_str(),   m_Diffuse);
    shader->SetUniformVec3((uniformBase + "specular").c_str(),  m_Specular);
    shader->SetUniformVec3((uniformBase + "position").c_str(),  m_Position);
    shader->  SetUniform1f((uniformBase + "constant").c_str(),  1.0f);
    shader->  SetUniform1f((uniformBase + "linear").c_str(),    0.09f);
    shader->  SetUniform1f((uniformBase + "quadratic").c_str(), 0.032f);
}