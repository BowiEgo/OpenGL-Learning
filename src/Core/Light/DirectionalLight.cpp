#include "DirectionalLight.h"

void DirectionalLight::SetID(unsigned int id)
{
    m_RendererID = id;
}

void DirectionalLight::Update(Shader* shader)
{
    shader->Bind();
    shader->SetUniformBool("u_DirectionalLight.enable",    m_IsEnabled);
    shader->SetUniformVec3("u_DirectionalLight.ambient",   m_Ambient);
    shader->SetUniformVec3("u_DirectionalLight.diffuse",   m_Diffuse);
    shader->SetUniformVec3("u_DirectionalLight.specular",  m_Specular);
    shader->SetUniformVec3("u_DirectionalLight.direction", m_Direction);
}
