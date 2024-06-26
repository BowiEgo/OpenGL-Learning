#include "Postprocessing.h"

#include "Core/Geometry/QuadGeometry.h"
#include "FileSystem/FileSystem.h"

Postprocessing::Postprocessing()
{
    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    fbSpec.HDR = true;
    m_Framebuffer = Framebuffer::Create(fbSpec);
    // texture
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    m_Texture = std::make_shared<Texture2D>("Texture_Diffuse", textureID);
    // shader
    std::string normalizedVertSrc = FileSystem::ReadFile("../res/shaders/Postprocessing.vert");
    std::string normalizedFragSrc = FileSystem::ReadFile("../res/shaders/Postprocessing.frag");
    m_Shader = std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc);
    // material
    m_Material = std::make_shared<ShaderMaterial>(m_Shader);
    m_Material->AddTexture(m_Texture);
    // mesh
    m_Mesh = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), m_Material);
}

void Postprocessing::Bind()
{
    m_Framebuffer->Bind();
}

void Postprocessing::Unbind()
{
    m_Framebuffer->Unbind();
}
