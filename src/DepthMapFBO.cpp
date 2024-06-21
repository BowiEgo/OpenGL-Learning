#include "DepthMapFBO.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::shared_ptr<DepthMapFBO> DepthMapFBO::Create(const FramebufferSpecification &spec)
{
    return std::make_shared<DepthMapFBO>(spec);
}

DepthMapFBO::DepthMapFBO(const FramebufferSpecification &spec)
    : m_Specification(spec)
{
    Invalidate();
}

DepthMapFBO::~DepthMapFBO()
{
    GLCall(glDeleteFramebuffers(1, &m_RendererID));
    GLCall(glDeleteTextures(1, &m_DepthMap));
}

void DepthMapFBO::Invalidate()
{
    if (m_RendererID)
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(1, &m_DepthMap));
    }

    GLCall(glGenFramebuffers(1, &m_RendererID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    GLCall(glGenTextures(1, &m_DepthMap));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthMap));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);) 
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0));
    GLCall(glDrawBuffer(GL_NONE));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void DepthMapFBO::Bind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
    GLCall(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}

void DepthMapFBO::Unbind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
