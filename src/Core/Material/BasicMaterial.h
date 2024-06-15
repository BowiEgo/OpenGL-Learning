#pragma once

#include "Material.h"

class BasicMaterial : public Material
{
public:
    BasicMaterial();
    virtual ~BasicMaterial() {};

    inline unsigned int GetType() const override { return m_Type; }

    void SetColor(const glm::vec3& color);

    virtual void BindShader() const override;
    virtual void UpdateShader(const glm::vec3& position, const glm::vec3& scale, std::pair<float, const glm::vec3>* rotation) const override;
    virtual void UpdateShaderUniform(const std::string& uniformName, const UniformValue& uniformValue) const override;
private:
    unsigned int m_Type = MATERIAL_TYPE_BASIC;
    Ref<Shader> m_Shader;
    glm::vec3 m_Color;
};