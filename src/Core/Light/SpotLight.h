#pragma once

#include <glm/glm.hpp>

#include "Light.h"
#include "Core/Shader.h"

class SpotLight : public Light
{
public:
    SpotLight() {};

    inline bool*     GetIsEnabled() { return &m_IsEnabled; }
    inline float*    GetAmbient()   { return m_Ambient; }
    inline float*    GetDiffuse()   { return m_Diffuse; }
    inline float*    GetSpecular()  { return m_Specular; }
    inline float*    GetPhi()  { return &m_Phi; }
    inline float*    GetCutOff()  { return &m_CutOff; }
    inline float*    GetSoftEdge()  { return &m_SoftEdge; }
    inline float*    GetOuterCutOff()  { return &m_OuterCutOff; }

    virtual void SetID(unsigned int id) override;
    virtual void Update(Shader* shader) override;

    void UpdateSpotLightCutOff();
private:
    unsigned int m_RendererID;
    bool m_IsEnabled = true;
    glm::vec3 m_Position;
    float m_Direction[3] = { -0.2f, -1.0f, -0.3f };

    float m_Ambient[3] = { 0.2f, 0.2f, 0.2f },
          m_Diffuse[3] = { 0.5f, 0.5f, 0.5f },
          m_Specular[3] = { 1.0f, 1.0f, 1.0f };

    float m_Phi = 16.0f;
    float m_CutOff = glm::cos(glm::radians(m_Phi));
    float m_SoftEdge = 1.5f;
    float m_OuterCutOff = glm::cos(glm::radians(m_Phi + m_SoftEdge));
};