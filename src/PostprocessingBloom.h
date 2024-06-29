#pragma once

#include "Framebuffer.h"
#include "Core/Scene.h"
#include "Core/Texture2D.h"
#include "Core/Material/ShaderMaterial.h"
#include "Core/Mesh.h"

class PostprocessingBloom
{
public:
    PostprocessingBloom();
    ~PostprocessingBloom() {};

    inline Mesh* GetScreen() { return m_ScreenMesh.get(); }
    inline ShaderMaterial* GetMaterial() { return m_Material.get(); }

    void Bind();
    void Unbind();
    void Render(Scene* scene);
private:
    Ref<Framebuffer> m_Framebuffer;
    std::vector<Ref<Framebuffer>> m_PingpongFramebuffers;
    Ref<Texture2D> m_PingpongTextureV, m_PingpongTextureH;
    GLuint m_ColorAttactments[2];
    Ref<Shader> m_Shader, m_PingpongShader;
    Ref<Texture2D> m_Texture, m_BrightTexture;
    Ref<ShaderMaterial> m_Material, m_PingpongMaterial;
    Ref<Mesh> m_PingpongMesh, m_ScreenMesh;

    GLuint pingpongFBOs[2];
    GLuint pingpongBuffers[2];
};
