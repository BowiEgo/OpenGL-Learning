#pragma once

#include "Material.h"
#include "Light.h"

class StandardMaterial : public Material
{
public:
    StandardMaterial(std::shared_ptr<Light> light);
    ~StandardMaterial();

    inline std::shared_ptr<Shader> GetShader() const override { return m_Shader; }
private:
    std::shared_ptr<Light> m_Light;
    std::shared_ptr<Shader> m_Shader;
};