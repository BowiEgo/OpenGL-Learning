#include "Light.h"

#include "FileSystem/FileSystem.h"

Light::Light(const std::shared_ptr<Camera> camera)
    : m_Camera(camera)
{
    std::string vertexSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.vert");
    std::string fragSrc = FileSystem::ReadFile("../res/shaders/ModelLoading.frag");
    m_Shader = std::make_shared<Shader>(vertexSrc, fragSrc);
}

Light::~Light()
{
}

void Light::UpdateShader(float* translate)
{
    glm::mat4 proj = m_Camera->GetProjMatrix();
    glm::mat4 view = m_Camera->GetViewMatrix();
    glm::mat4 model(1.0f);
    if (translate != nullptr)
        model = glm::translate(model, glm::vec3(translate[0], translate[1], translate[2]));

    m_Shader->Bind();
    m_Shader->SetUniformMat4("projectionMatrix", proj);
    m_Shader->SetUniformMat4("viewMatrix",       view);
    m_Shader->SetUniformMat4("modelMatrix",      model);
    m_Shader->  SetUniform1f("u_Material.shininess",        m_MaterialShininess);

    m_Shader->SetUniformBool("u_DirectionalLight.enable",    m_IsEnabled_DirectionalLight);
    m_Shader->SetUniformVec3("u_DirectionalLight.ambient",   m_LightAmbient);
    m_Shader->SetUniformVec3("u_DirectionalLight.diffuse",   m_LightDiffuse);
    m_Shader->SetUniformVec3("u_DirectionalLight.specular",  m_LightSpecular);
    m_Shader->SetUniformVec3("u_DirectionalLight.direction", m_DirectionalLightDir);

    m_Shader->  SetUniform1i("u_NR_PointLights",  m_PointLightPositions.size());
    for (int i = 0; i < m_PointLightPositions.size(); i++)
    {
        std::string uniformBase = "u_PointLights[" + std::to_string(i) + "].";
        m_Shader->SetUniformBool((uniformBase + "enable").c_str(),    m_IsEnabled_PointLight);
        m_Shader->SetUniformVec3((uniformBase + "ambient").c_str(),   m_LightAmbient);
        m_Shader->SetUniformVec3((uniformBase + "diffuse").c_str(),   m_LightDiffuse);
        m_Shader->SetUniformVec3((uniformBase + "specular").c_str(),  m_LightSpecular);
        m_Shader->SetUniformVec3((uniformBase + "position").c_str(),  m_PointLightPositions[i]);
        m_Shader->  SetUniform1f((uniformBase + "constant").c_str(),  1.0f);
        m_Shader->  SetUniform1f((uniformBase + "linear").c_str(),    0.09f);
        m_Shader->  SetUniform1f((uniformBase + "quadratic").c_str(), 0.032f);
    }

    m_Shader->SetUniformBool("u_SpotLight.enable",      m_IsEnabled_SpotLight);
    m_Shader->SetUniformVec3("u_SpotLight.ambient",     m_LightAmbient);
    m_Shader->SetUniformVec3("u_SpotLight.diffuse",     m_LightDiffuse);
    m_Shader->SetUniformVec3("u_SpotLight.specular",    m_LightSpecular);
    m_Shader->SetUniformVec3("u_SpotLight.position",    m_Camera->GetPosition());
    m_Shader->SetUniformVec3("u_SpotLight.direction",   m_Camera->GetFront());
    m_Shader->  SetUniform1f("u_SpotLight.cutOff",      m_SpotLightCutOff);
    m_Shader->  SetUniform1f("u_SpotLight.outerCutOff", m_SpotLightOuterCutOff);
    m_Shader->  SetUniform1f("u_SpotLight.constant",  1.0f);
    m_Shader->  SetUniform1f("u_SpotLight.linear",    0.09f);
    m_Shader->  SetUniform1f("u_SpotLight.quadratic", 0.032f);
}

void Light::UpdateSpotLightCutOff()
{
    m_SpotLightCutOff = glm::cos(glm::radians(m_SpotLightPhi));
    m_SpotLightOuterCutOff = glm::cos(glm::radians(m_SpotLightPhi + m_SpotLight_SoftEdge));
}
