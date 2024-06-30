#include "GBuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GBuffer::GBuffer(const GBufferSpecification& spec)
    : m_Specification(spec)
{
    Invalidate();
}

GBuffer::~GBuffer()
{
    GLCall(glDeleteFramebuffers(1, &m_RendererID));
    GLCall(glDeleteTextures(1, &m_GPosition));
    GLCall(glDeleteTextures(1, &m_GNormal));
    GLCall(glDeleteTextures(1, &m_GAlbedoSpec));
    GLCall(glDeleteRenderbuffers(1, &m_Depth_Attachment));
}

std::shared_ptr<GBuffer> GBuffer::Create(const GBufferSpecification &spec)
{
    return std::make_shared<GBuffer>(spec);
}

void GBuffer::Invalidate()
{
    if (m_RendererID)
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(1, &m_GPosition));
        GLCall(glDeleteTextures(1, &m_GNormal));
        GLCall(glDeleteTextures(1, &m_GAlbedoSpec));
        GLCall(glDeleteRenderbuffers(1, &m_Depth_Attachment));
    }

    GLCall(glGenFramebuffers(1, &m_RendererID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    // - Position color buffer
    GLCall(glGenTextures(1, &m_GPosition));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_GPosition));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GPosition, 0));

    // - Normal color buffer
    GLCall(glGenTextures(1, &m_GNormal));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_GNormal));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GNormal, 0));

    // - Color + Specular color buffer
    GLCall(glGenTextures(1, &m_GAlbedoSpec));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_GAlbedoSpec));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GAlbedoSpec, 0));

    // - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    GLCall(glDrawBuffers(3, attachments));

    // - Create and attach depth buffer (renderbuffer)
    GLCall(glGenRenderbuffers(1, &m_Depth_Attachment));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_Depth_Attachment));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Depth_Attachment));

    // - Finally check if framebuffer is complete
    CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GBuffer::Bind()
{
    GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID));
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID));
    GLCall(glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
    GLCall(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
}

void GBuffer::Unbind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
