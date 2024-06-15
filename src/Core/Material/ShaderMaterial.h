#pragma once

#include "Material.h"

#include "Core/Texture2D.h"

class ShaderMaterial : public Material
{
public:
    ShaderMaterial(Ref<Shader> shader);
    ~ShaderMaterial() {};

    inline unsigned int GetType() const override { return m_Type; }

    void AddTexture(Ref<Texture2D> texture);
    void SetTextures(std::vector<Ref<Texture2D>> textures);

    virtual void BindShader() const override;
    virtual void UpdateShader(const glm::vec3& position, const glm::vec3& scale, std::pair<float, const glm::vec3>* rotation) const override;
    virtual void UpdateShaderUniform(const std::string& uniformName, const UniformValue& uniformValue) const override;
private:
    unsigned int m_Type = MATERIAL_TYPE_SHADER;
    Ref<Shader> m_Shader;
    std::vector<Ref<Texture2D>> m_Textures;
};