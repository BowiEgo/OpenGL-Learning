#include "PostprocessingBloom.h"

#include "Core/Geometry/QuadGeometry.h"
#include "FileSystem/FileSystem.h"

PostprocessingBloom::PostprocessingBloom()
{
    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    fbSpec.HDR = true;

    glGenFramebuffers(2, pingpongFBOs);
    glGenTextures(2, pingpongBuffers);
    for (GLuint i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBOs[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffers[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB16F, fbSpec.Width, fbSpec.Height, 0, GL_RGB, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffers[i], 0
        );
    }

    m_Framebuffer = Framebuffer::Create(fbSpec);
    m_PingpongFramebuffers = {
        Framebuffer::Create(fbSpec),
        Framebuffer::Create(fbSpec)
    };
    m_PingpongTextureV = std::make_shared<Texture2D>("Texture_Diffuse", pingpongBuffers[0]);
    m_PingpongTextureH = std::make_shared<Texture2D>("Texture_Diffuse", pingpongBuffers[1]);
    // --------------------
    // Pingpong
    // --------------------
    // texture
    uint32_t* textureIDs = m_Framebuffer->GetColorAttachments();
    m_Texture = std::make_shared<Texture2D>("Texture_Diffuse", textureIDs[0]);
    m_BrightTexture = std::make_shared<Texture2D>("Texture_Diffuse", textureIDs[1]);
    // shader
    std::string pingpongVertSrc = FileSystem::ReadFile("../res/shaders/Pingpong.vert");
    std::string pingpongFragSrc = FileSystem::ReadFile("../res/shaders/Pingpong.frag");
    m_PingpongShader = std::make_shared<Shader>(pingpongVertSrc, pingpongFragSrc);
    // material
    m_PingpongMaterial = std::make_shared<ShaderMaterial>(m_PingpongShader);
    m_PingpongMesh = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), m_PingpongMaterial);
    // --------------------
    // Screen
    // --------------------
    // shader
    std::string normalizedVertSrc = FileSystem::ReadFile("../res/shaders/Postprocessing.vert");
    std::string normalizedFragSrc = FileSystem::ReadFile("../res/shaders/Postprocessing.frag");
    m_Shader = std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc);
    // material
    m_Material = std::make_shared<ShaderMaterial>(m_Shader);
    m_Material->SetTextures({ m_Texture, m_PingpongTextureH });
    m_Material->UpdateShaderUniform("u_Normalized_Type", -1);
    // mesh
    m_ScreenMesh = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), m_Material);
}

void PostprocessingBloom::Bind()
{
    m_Framebuffer->Bind();
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void PostprocessingBloom::Unbind()
{
    m_Framebuffer->Unbind();
}

void PostprocessingBloom::Render(Scene* scene)
{
    GLboolean horizontal = true, first_iteration = true;
    GLuint amount = 10;

    for (GLuint i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBOs[horizontal]); 
        m_PingpongMaterial->UpdateShaderUniform("horizontal", horizontal);
        if (first_iteration)
        {
            m_PingpongMaterial->SetTextures({ m_BrightTexture });
        }
        else
        {
            m_PingpongMaterial->SetTextures({ horizontal ? m_PingpongTextureV : m_PingpongTextureH });
        }
        scene->Draw(m_PingpongMesh.get());
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
