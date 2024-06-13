#pragma once

#include "pch.h"

#include "Core/Shader/Shader.h"
#include "Core/Texture2D.h"
#include "Core/TextureCubemap.h"

#define MATERIAL_TYPE_NONE 0
#define MATERIAL_TYPE_BASIC 1
#define MATERIAL_TYPE_STANDARD 2
#define MATERIAL_TYPE_SHADER 3
#define MATERIAL_TYPE_CUBEMAP 4

class Material
{
public:
    Material() {};
    virtual ~Material() {};

    virtual inline unsigned int GetType() const = 0;
    virtual inline std::vector<Ref<Texture2D>> GetTextures() const { return m_Textures; }

    virtual void BindShader() const = 0;
    virtual void UpdateShader(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation) const = 0;
    virtual void UpdateShaderUniform(const std::string& uniformName, const UniformValue& uniformValue) const = 0;
    void Update(glm::vec3& position, glm::vec3& scale, std::pair<float, glm::vec3>* rotation);
public:
    bool Wireframe_Enabled = false;
private:
    unsigned int m_Type = MATERIAL_TYPE_NONE;
    Ref<Shader> m_Shader;
    std::vector<Ref<Texture2D>> m_Textures;
};