#pragma once

#include "Material.h"

class BasicMaterial : public Material
{
public:
    BasicMaterial();
    virtual ~BasicMaterial();

    inline unsigned int GetType() const override { return m_Type; }

    void SetColor(const glm::vec3& color);
private:
    unsigned int m_Type = MATERIAL_TYPE_BASIC;
    std::shared_ptr<Shader> m_Shader;
    glm::vec3 m_Color;
};