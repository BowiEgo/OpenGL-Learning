#pragma once

#include <glm/glm.hpp>

#include "Light.h"
#include "Core/Shader.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight() {};

    inline float* GetDirection() { return m_Direction; }
    inline bool*  GetIsEnabled() { return &m_IsEnabled; }
    inline float* GetAmbient()   { return m_Ambient; }
    inline float* GetDiffuse()   { return m_Diffuse; }
    inline float* GetSpecular()  { return m_Specular; }

    void SetPositions(float position[3]);

    virtual void SetID(unsigned int id) override;
    virtual void Update(Shader* shader) override;
private:
    unsigned int m_RendererID;
    bool m_IsEnabled = true;
    float m_Direction[3] = { -0.2f, -1.0f, -0.3f };

    float m_Ambient[3] = { 0.2f, 0.2f, 0.2f },
          m_Diffuse[3] = { 0.5f, 0.5f, 0.5f },
          m_Specular[3] = { 1.0f, 1.0f, 1.0f };
};