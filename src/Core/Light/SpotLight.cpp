#include "SpotLight.h"

#include "Core/Scene.h"

void SpotLight::SetID(unsigned int id)
{
    m_RendererID = id;
}

void SpotLight::Update(Shader *shader)
{
    std::string uniformBase = "u_SpotLights[" + std::to_string(m_RendererID) + "].";

    m_Position = Scene::GetCurrentCamera()->GetPosition();
    m_Direction[0] = Scene::GetCurrentCamera()->GetFront().x;
    m_Direction[1] = Scene::GetCurrentCamera()->GetFront().y;
    m_Direction[2] = Scene::GetCurrentCamera()->GetFront().z;

    shader->Bind();
    shader->SetUniformBool("u_SpotLight.enable",      m_IsEnabled);
    shader->SetUniformVec3("u_SpotLight.ambient",     m_Ambient);
    shader->SetUniformVec3("u_SpotLight.diffuse",     m_Diffuse);
    shader->SetUniformVec3("u_SpotLight.specular",    m_Specular);
    shader->SetUniformVec3("u_SpotLight.position",    m_Position);
    shader->SetUniformVec3("u_SpotLight.direction",   m_Direction);
    shader->  SetUniform1f("u_SpotLight.cutOff",      m_CutOff);
    shader->  SetUniform1f("u_SpotLight.outerCutOff", m_OuterCutOff);
    shader->  SetUniform1f("u_SpotLight.constant",  1.0f);
    shader->  SetUniform1f("u_SpotLight.linear",    0.09f);
    shader->  SetUniform1f("u_SpotLight.quadratic", 0.032f);
}

void SpotLight::UpdateSpotLightCutOff()
{
    m_CutOff = glm::cos(glm::radians(m_Phi));
    m_OuterCutOff = glm::cos(glm::radians(m_Phi + m_SoftEdge));
}
