#pragma once

#include "Material.h"

#include "Core/TextureCubemap.h"

class CubemapMaterial : public Material
{
public:
    CubemapMaterial();
    ~CubemapMaterial() {};

    inline unsigned int GetType() const override { return m_Type; }
    inline Ref<TextureCubemap> GetCubemapTexture() const { return m_CubemapTexture; }

    void SetCubemapTexture(Ref<TextureCubemap> texture);

    void BindTexture(const unsigned int& slot = 0) const;
    virtual void BindShader() const override;
    virtual void UpdateShader(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation) const override;
    virtual void UpdateShaderUniform(const std::string& uniformName, const UniformValue& uniformValue) const override;
private:
    unsigned int m_Type = MATERIAL_TYPE_CUBEMAP;
    Ref<Shader> m_Shader;
    Ref<TextureCubemap> m_CubemapTexture;
};