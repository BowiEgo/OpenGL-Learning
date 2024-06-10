#pragma once

#include "Material.h"

#include "Core/Texture2D.h"

class StandardMaterial : public Material
{
public:
    StandardMaterial();
    ~StandardMaterial() {};

    inline unsigned int GetType() const override { return m_Type; }

    void SetDiffuseTexture(Ref<Texture2D> texture);
    void SetSpecularTexture(Ref<Texture2D> texture);

    void BindTexture() const;
    virtual void BindShader() const override;
    virtual void UpdateShader(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation) const override;
    virtual void UpdateShaderUniform(const std::string& uniformName, const UniformValue& uniformValue) const override;
private:
    unsigned int m_Type = MATERIAL_TYPE_STANDARD;
    Ref<Shader> m_Shader;
    Ref<Texture2D> m_DiffuseTexture;
    Ref<Texture2D> m_SpecularTexture;

    float m_MaterialShininess = 32.0f, m_MaterialEmmisionStength = 1.0f;
};