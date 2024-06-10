#pragma once

#include "pch.h"

#include "Core/Shader/Shader.h"

#define MATERIAL_TYPE_NONE 0
#define MATERIAL_TYPE_BASIC 1
#define MATERIAL_TYPE_STANDARD 2
#define MATERIAL_TYPE_SHADER 3

class Mesh;

class Material
{
public:
    Material() {};
    virtual ~Material() {};

    virtual inline unsigned int GetType() const = 0;
    virtual void BindShader() const = 0;
    virtual void UpdateShader(float* position, float* scale, std::pair<float, glm::vec3>* rotation) const = 0;
    virtual void UpdateShaderUniform(const std::string& uniformName, const UniformValue& uniformValue) const = 0;
private:
    std::shared_ptr<Shader> m_Shader;
    unsigned int m_Type = MATERIAL_TYPE_NONE;
};