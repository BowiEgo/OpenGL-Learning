#pragma once

#include <glm/glm.hpp>

#include "Light.h"
#include "Core/Shader/Shader.h"

class PointLight : public Light
{
public:
    PointLight() = default;
    PointLight(float position[3]);
    PointLight(const glm::vec3& position);

    inline bool*  GetIsEnabled() { return &m_IsEnabled; }
    inline float* GetPositions() { return m_Position; }
    inline float* GetAmbient()   { return m_Ambient; }
    inline float* GetDiffuse()   { return m_Diffuse; }
    inline float* GetSpecular()  { return m_Specular; }

    void SetEnabled(bool& value);
    void SetPositions(float position[3]);
    void SetPositions(glm::vec3& position);

    virtual void SetID(unsigned int id) override;
    virtual void Update(Shader* shader) override;
private:
    unsigned int m_RendererID;
    bool m_IsEnabled = true;
    float m_Position[3] = { 0.0f, 0.0f, 0.0f };

    float m_Ambient[3] = { 0.2f, 0.2f, 0.2f },
          m_Diffuse[3] = { 0.5f, 0.5f, 0.5f },
          m_Specular[3] = { 1.0f, 1.0f, 1.0f };
};