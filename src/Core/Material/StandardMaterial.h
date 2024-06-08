#pragma once

#include "Material.h"

#include "Core/Texture2D.h"

class StandardMaterial : public Material
{
public:
    StandardMaterial();
    ~StandardMaterial();

    inline unsigned int GetType() const override { return m_Type; }

    void SetDiffuseTexture(std::shared_ptr<Texture2D> texture);
    void SetSpecularTexture(std::shared_ptr<Texture2D> texture);

    void BindTexture();
private:
    unsigned int m_Type = MATERIAL_TYPE_STANDARD;
    std::shared_ptr<Texture2D> m_DiffuseTexture;
    std::shared_ptr<Texture2D> m_SpecularTexture;

    float m_MaterialShininess = 32.0f, m_MaterialEmmisionStength = 1.0f;
};