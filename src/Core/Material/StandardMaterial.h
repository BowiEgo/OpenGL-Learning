#pragma once

#include "Material.h"

#include "Core/Texture2D.h"

class StandardMaterial : public Material
{
public:
    StandardMaterial();
    ~StandardMaterial();

    inline unsigned int GetType() const override { return m_Type; }

    void SetDiffuseTexture(Texture2D* texture);
    void SetSpecularTexture(Texture2D* texture);

    void BindTexture();
private:
    unsigned int m_Type = MATERIAL_TYPE_STANDARD;
    Texture2D* m_DiffuseTexture;
    Texture2D* m_SpecularTexture;

    float m_MaterialShininess = 32.0f, m_MaterialEmmisionStength = 1.0f;
};