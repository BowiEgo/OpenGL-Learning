#pragma once

#include "Material.h"

class BasicMaterial : public Material
{
public:
    BasicMaterial();
    virtual ~BasicMaterial();

    inline std::shared_ptr<Shader> GetShader() const override { return m_Shader; }

    void SetColor(const float vector[3]) override;
private:
    std::shared_ptr<Shader> m_Shader;
};