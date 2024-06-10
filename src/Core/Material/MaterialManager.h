#pragma once

#include "pch.h"

#include "ShaderMaterial.h"

class MaterialManager
{
public:
    MaterialManager() {};
    ~MaterialManager() {};

    static MaterialManager* Create();

    inline static Ref<ShaderMaterial> GetOutlineMaterial() { return s_Instance->m_OutlineMaterial; }
private:
    static MaterialManager* s_Instance;
    Ref<ShaderMaterial> m_OutlineMaterial;
};