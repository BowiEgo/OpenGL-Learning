#pragma once

#include "Core/Shader.h"

#include <memory>

#define MATERIAL_TYPE_NONE 0
#define MATERIAL_TYPE_BASIC 1
#define MATERIAL_TYPE_STANDARD 2

class Material
{
public:
    Material() {};
    virtual ~Material() {};

    virtual inline unsigned int GetType() const = 0;
private:
    std::shared_ptr<Shader> m_Shader;
    unsigned int m_Type = MATERIAL_TYPE_NONE;
};