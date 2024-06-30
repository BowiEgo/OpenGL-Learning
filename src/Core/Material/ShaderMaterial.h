#pragma once

#include "Material.h"

#include "Core/Texture.h"

class ShaderMaterial : public Material
{
public:
    ShaderMaterial(Ref<Shader> shader);
    ~ShaderMaterial() {};

    inline unsigned int GetType() const override { return m_Type; }
    inline Shader* GetShader() const { return m_Shader.get(); }

    void AddTexture(Ref<Texture2D> texture);
    void AddTexture(const std::string& name, const Ref<Texture2D> texture);
    void SetTextures(std::vector<Ref<Texture2D>> textures);

    virtual void BindShader() const override;
    virtual void UpdateShader(const glm::vec3& position, const glm::vec3& scale, const std::pair<float, glm::vec3>* rotation) const override;
    virtual void UpdateShaderUniform(const std::string& uniformName, const UniformValue& uniformValue) const override;
private:
    unsigned int m_Type = MATERIAL_TYPE_SHADER;
    Ref<Shader> m_Shader;
    std::vector<Ref<Texture2D>> m_Textures;
    std::vector<std::pair<std::string, Ref<Texture2D>>> m_Named_Textures;
};