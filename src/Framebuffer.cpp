#include "Framebuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
{
    return std::make_shared<Framebuffer>(spec);
}

Framebuffer::Framebuffer(const FramebufferSpecification & spec)
    : m_Specification(spec)
{
    Invalidate();
}

Framebuffer::~Framebuffer()
{
    GLCall(glDeleteFramebuffers(1, &m_RendererID));
    GLCall(glDeleteTextures(1, &m_ColorAttachment));
    GLCall(glDeleteRenderbuffers(1, &m_DepthAttachment));
}

void Framebuffer::Invalidate()
{
    if (m_RendererID)
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(1, &m_ColorAttachment));
        GLCall(glDeleteRenderbuffers(1, &m_DepthAttachment));
    }

    GLCall(glGenFramebuffers(1, &m_RendererID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    // Create color attachment
    GLCall(glGenTextures(1, &m_ColorAttachment));
    if (m_Specification.Samples == 1)
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
        if (m_Specification.HDR)
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_FLOAT, NULL));
        }
        else
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        }
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));	// we only need a color buffer
    }
    else
    {
        GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorAttachment));
        if (m_Specification.HDR)
        {
            GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB16F, m_Specification.Width, m_Specification.Height, GL_TRUE));
        }
        else
        {
            GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, m_Specification.Width, m_Specification.Height, GL_TRUE));
        }
        GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorAttachment, 0));
    }

    // Create depth attachment
    GLCall(glGenRenderbuffers(1, &m_DepthAttachment));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment));
    if (m_Specification.Samples == 1)
    {
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height));
    }
    else
    {
        GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Specification.Samples, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height));
    }
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0)); // Unbind renderbuffer once

    CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    // Unbind the framebuffer
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::Bind()
{
    // GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
    GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID));
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID));
    GLCall(glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
    GLCall(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
}

void Framebuffer::Unbind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
    m_Specification.Width = width;
    m_Specification.Height = height;

    Invalidate();
}

FramebufferManager* FramebufferManager::s_Instance = nullptr;
bool FramebufferManager::s_SMAA_Enabled = false;

Ref<Framebuffer> FramebufferManager::CreateFramebuffer(std::string tag, const FramebufferSpecification &spec)
{
    Ref<Framebuffer> fb = std::make_shared<Framebuffer>(spec);
    s_Instance->m_Pools.push_back(std::make_pair(tag, fb));
    return fb;
}

Ref<Framebuffer> FramebufferManager::GetByTag(const std::string &tag)
{
    for (const auto& pair : s_Instance->m_Pools)
    {
        if (pair.first == tag)
        {
            return pair.second;
        }
    }
    return nullptr;
}

bool FramebufferManager::Remove(const std::string &tag)
{
    auto it = std::remove_if(s_Instance->m_Pools.begin(), s_Instance->m_Pools.end(),
        [&tag](const std::pair<std::string, Ref<Framebuffer>>& pair)
        {
            return pair.first == tag;
        });

    if (it != s_Instance->m_Pools.end())
    {
        s_Instance->m_Pools.erase(it, s_Instance->m_Pools.end());
        return true;
    }
    return false;
}
