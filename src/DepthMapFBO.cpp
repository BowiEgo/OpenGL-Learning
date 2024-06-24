#include "DepthMapFBO.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Texture2D.h"
#include "Core/TextureCubemap.h"

std::shared_ptr<DepthMapFBO> DepthMapFBO::Create(const FramebufferSpecification &spec)
{
    return std::make_shared<DepthMapFBO>("Texture2D", spec);
}

std::shared_ptr<DepthMapFBO> DepthMapFBO::Create(const std::string textureType, const FramebufferSpecification &spec)
{
    return std::make_shared<DepthMapFBO>(textureType, spec);
}

DepthMapFBO::DepthMapFBO(const std::string textureType, const FramebufferSpecification &spec)
    : m_TextureType(textureType), m_Specification(spec)
{
    Invalidate();
}

DepthMapFBO::~DepthMapFBO()
{
    GLCall(glDeleteFramebuffers(1, &m_RendererID));
    GLCall(glDeleteTextures(1, &m_ColorAttachment));
    GLCall(glDeleteTextures(1, &m_DepthAttachment));
}

void DepthMapFBO::Invalidate()
{
    if (m_RendererID)
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(1, &m_ColorAttachment));
        GLCall(glDeleteTextures(1, &m_DepthAttachment));
    }

    GLCall(glGenFramebuffers(1, &m_RendererID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    // Create color attachment
    // GLCall(glGenTextures(1, &m_ColorAttachment));
    // GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
    // GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    // GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));	// we only need a color buffer

    // Create depth attachment
    GLCall(glGenTextures(1, &m_DepthAttachment));
    if (m_TextureType == "Texture2D")
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                    m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);) 
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0));
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
    else if (m_TextureType == "TextureCubemap")
    {
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthAttachment));
        for (GLuint i = 0; i < 6; ++i)
        {
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
        }

        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthAttachment, 0));
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }

    CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void DepthMapFBO::Bind()
{
    GLCall(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}

void DepthMapFBO::Unbind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
