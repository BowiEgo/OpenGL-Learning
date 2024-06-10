#pragma once

#include "Material.h"

#include "Core/Texture2D.h"

class ShaderMaterial : public Material
{
public:
    ShaderMaterial(Ref<Shader> shader);
    ~ShaderMaterial() {};

    inline unsigned int GetType() const override { return m_Type; }

    void SetDiffuseTexture(Ref<Texture2D> texture);
    void SetSpecularTexture(Ref<Texture2D> texture);

    void BindTexture() const;
    virtual void BindShader() const override;
    virtual void UpdateShader(float* position, float* scale, std::pair<float, glm::vec3>* rotation) const override;
    virtual void UpdateShaderUniform(const std::string& uniformName, const UniformValue& uniformValue) const override;
private:
    unsigned int m_Type = MATERIAL_TYPE_SHADER;
    Ref<Shader> m_Shader;
    Ref<Texture2D> m_DiffuseTexture;
    Ref<Texture2D> m_SpecularTexture;
};