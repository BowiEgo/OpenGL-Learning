#include "StandardMaterial.h"

StandardMaterial::StandardMaterial(std::shared_ptr<Light> light)
    : m_Light(light)
{
    m_Shader = m_Light->GetShader();
}

StandardMaterial::~StandardMaterial()
{
}
