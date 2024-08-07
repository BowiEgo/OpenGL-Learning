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
    inline glm::vec3& GetPositions() { return m_Position; }
    inline float* GetAmbient()   { return m_Ambient; }
    inline float* GetDiffuse()   { return m_Diffuse; }
    inline float* GetSpecular()  { return m_Specular; }

    void SetEnabled(bool& value);
    void SetPosition(const std::array<float, 3>& position);
    void SetPosition(glm::vec3& position);
    void SetAmbient(const std::array<float, 3>& ambient);
    void SetColor(const std::array<float, 3>& color);
    void SetColor(glm::vec3& color);

    virtual void SetID(unsigned int id) override;
    virtual void Update(Shader* shader) override;
private:
    unsigned int m_RendererID;
    bool m_IsEnabled = true;
    glm::vec3 m_Position = glm::vec3(0.0f);

    float m_Ambient[3] = { 0.02f, 0.02f, 0.02f },
          m_Diffuse[3] = { 0.5f, 0.5f, 0.5f },
          m_Specular[3] = { 1.0f, 1.0f, 1.0f };

    glm::vec3 m_Color = glm::vec3(1.0f);
};