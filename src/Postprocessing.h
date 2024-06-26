#pragma once

#include "Framebuffer.h"
#include "Core/Texture2D.h"
#include "Core/Material/ShaderMaterial.h"
#include "Core/Mesh.h"

class Postprocessing
{
public:
    Postprocessing();
    ~Postprocessing() {};

    inline Mesh* GetScreen() { return m_Mesh.get(); }
    inline ShaderMaterial* GetMaterial() { return m_Material.get(); }

    void Bind();
    void Unbind();
private:
    Ref<Framebuffer> m_Framebuffer;
    Ref<Texture2D> m_Texture;
    Ref<Shader> m_Shader;
    Ref<ShaderMaterial> m_Material;
    Ref<Mesh> m_Mesh;
};
