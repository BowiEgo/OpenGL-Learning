#pragma once

#include <glm/glm.hpp>

#include "Light.h"
#include "Core/Shader/Shader.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight() {};

    inline glm::vec3* GetDirection() { return &m_Direction; }
    inline bool*  GetIsEnabled() { return &m_IsEnabled; }
    inline float* GetAmbient()   { return m_Ambient; }
    inline float* GetDiffuse()   { return m_Diffuse; }
    inline float* GetSpecular()  { return m_Specular; }

    virtual void SetID(unsigned int id) override;
    virtual void Update(Shader* shader) override;

    void SetDirection(glm::vec3& direction);
private:
    unsigned int m_RendererID;
    bool m_IsEnabled = true;
    glm::vec3 m_Direction = glm::vec3(-0.2f, -1.0f, -0.3f);

    float m_Ambient[3] = { 0.2f, 0.2f, 0.2f },
          m_Diffuse[3] = { 0.5f, 0.5f, 0.5f },
          m_Specular[3] = { 1.0f, 1.0f, 1.0f };
};