#pragma once

#include "Material.h"

#include "Core/Texture2D.h"

class ShaderMaterial : public Material
{
public:
    ShaderMaterial(Ref<Shader> shader);
    ~ShaderMaterial();

    inline unsigned int GetType() const override { return m_Type; }
    inline Ref<Shader> GetShader() const { return m_Shader; }

    void SetDiffuseTexture(std::shared_ptr<Texture2D> texture);
    void SetSpecularTexture(std::shared_ptr<Texture2D> texture);

    void BindTexture();
private:
    unsigned int m_Type = MATERIAL_TYPE_SHADER;
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Texture2D> m_DiffuseTexture;
    std::shared_ptr<Texture2D> m_SpecularTexture;
};