#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "Camera.h"
#include "Shader.h"

class Light
{
public:
    Light(const std::shared_ptr<Camera> camera);
    ~Light();

    void UpdateShader(float* translate = nullptr);

    inline std::shared_ptr<Shader> GetShader() { m_Shader->Bind(); return m_Shader; }
    inline glm::mat4 GetProjMatrix() { return m_Camera->GetProjMatrix(); }
    inline glm::mat4 GetViewMatrix() { return m_Camera->GetViewMatrix(); }
    inline glm::vec3 GetCamPosition() { return m_Camera->GetPosition(); }
    inline glm::vec3 GetCamDirection() { return m_Camera->GetFront(); }
    inline float* GetMaterialShininess() { return &m_MaterialShininess; }
    inline float* GetLightAmbient() { return m_LightAmbient; }
    inline float* GetLightDiffuse() { return m_LightDiffuse; }
    inline float* GetLightSpecular() { return m_LightSpecular; }
    inline bool* GetDirectionalLightIsEnabled() { return &m_IsEnabled_DirectionalLight; }
    inline float* GetDirectionalLightDir() { return m_DirectionalLightDir; }
    inline bool* GetPointLightIsEnabled() { return &m_IsEnabled_PointLight; }
    inline std::vector<glm::vec3> GetPointLightPositions() { return m_PointLightPositions; }
    inline bool* GetSpotLightIsEnabled() { return &m_IsEnabled_SpotLight; }
    inline float* GetSpotLightPhi() { return &m_SpotLightPhi; }
    inline float* GetSpotLightCutOff() { return &m_SpotLightCutOff; }
    inline float* GetSpotLight_SoftEdge() { return &m_SpotLight_SoftEdge; }
    inline float* GetSpotLightOuterCutOff() { return &m_SpotLightOuterCutOff; }

    void UpdateSpotLightCutOff();
private:
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Camera> m_Camera;
    float m_MaterialShininess = 32.0f;

    float m_LightAmbient[3] = { 0.2f, 0.2f, 0.2f },
            m_LightDiffuse[3] = { 0.5f, 0.5f, 0.5f },
            m_LightSpecular[3] = { 1.0f, 1.0f, 1.0f };

    bool m_IsEnabled_DirectionalLight = true;
    float m_DirectionalLightDir[3] = { -0.2f, -1.0f, -0.3f };

    bool m_IsEnabled_PointLight = true;
    std::vector<glm::vec3> m_PointLightPositions = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    bool m_IsEnabled_SpotLight = true;
    float m_SpotLightPhi = 16.0f;
    float m_SpotLightCutOff = glm::cos(glm::radians(m_SpotLightPhi));
    float m_SpotLight_SoftEdge = 1.5f;
    float m_SpotLightOuterCutOff = glm::cos(glm::radians(m_SpotLightPhi + m_SpotLight_SoftEdge));
};