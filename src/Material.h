#pragma once

#include "Shader.h"

#include <memory>

class Material
{
public:
    Material();
    ~Material();

    virtual std::shared_ptr<Shader> GetShader() const = 0;

    virtual void SetColor(const float vector[3]) {};
private:
    std::shared_ptr<Shader> m_Shader;
};