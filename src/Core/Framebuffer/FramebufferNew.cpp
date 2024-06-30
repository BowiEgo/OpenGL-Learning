#include "FramebufferNew.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

FramebufferNew::FramebufferNew(const FramebufferSpecificationNew& spec)
    : m_Specification(spec)
{
    if (m_RendererID)
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
    }

    GLCall(glGenFramebuffers(1, &m_RendererID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    // Unbind the framebuffer
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FramebufferNew::~FramebufferNew()
{
    GLCall(glDeleteFramebuffers(1, &m_RendererID));
}

void FramebufferNew::AttachColorTexture(Ref<FramebufferTexture> texture)
{
    texture->Bind();
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_ColorAttachments.size(), GL_TEXTURE_2D, texture->GetID(), 0));	// we only need a color buffer
    m_ColorAttachments.push_back(texture);
}

void FramebufferNew::Bind()
{
    GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID));
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID));
    GLCall(glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
    GLCall(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
}

void FramebufferNew::Unbind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}