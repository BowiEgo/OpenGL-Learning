#pragma once

#include "Material.h"

#include "Core/Texture2D.h"


class StandardMaterial : public Material
{
public:
    StandardMaterial();
    ~StandardMaterial() {};

    inline unsigned int GetType() const override { return m_Type; }

    void Setup();

    void BindVertexShader(const std::string& filepath);
    void BindFragmentShader(const std::string& filepath);
    void BindGeometryShader(const std::string& filepath);
    void AddTexture(Ref<Texture2D> texture);
    void SetTextures(std::vector<Ref<Texture2D>> textures);
    void SetEnvironmentReflectiveRate(const float& rate);
    void SetEnvironmentRefractiveRate(const float& rate);
    void SetEnvironmentRefractiveIndex(const float& index);

    virtual void BindShader() const override;
    virtual void UpdateShader(const glm::vec3& position, const glm::vec3& scale, const std::pair<float, glm::vec3>* rotation) const override;
    virtual void UpdateShaderUniform(const std::string& uniformName, const UniformValue& uniformValue) const override;
    void BindTextures() const;
public:
    bool Is_Opaque = true;
    bool Discard_Transparent = false;
    bool Environment_Enabled = false;
private:
    static std::string s_VertSrc, s_FragSrc;
    std::string m_VertSrc, m_FragSrc, m_GeomSrc = "";
    unsigned int m_Type = MATERIAL_TYPE_STANDARD;
    Ref<Shader> m_Shader;
    std::vector<Ref<Texture2D>> m_Diffuse_Textures;
    std::vector<Ref<Texture2D>> m_Specular_Textures;
    std::vector<Ref<Texture2D>> m_Normal_Textures;
    std::vector<Ref<Texture2D>> m_Height_Textures;
    Ref<TextureCubemap> m_Env_Texture;

    float m_MaterialShininess = 32.0f, m_MaterialEmmisionStength = 1.0f;
    float m_Environment_Reflective_Rate = 0.2f;
    float m_Environment_Refractive_Rate = 0.2f;
    float m_Environment_Refractive_Index = 1.52f;
};