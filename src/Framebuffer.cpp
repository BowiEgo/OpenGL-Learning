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
    GLCall(glDeleteTextures(1, &m_DepthAttachment));
}

void Framebuffer::Invalidate()
{
    if (m_RendererID)
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(1, &m_ColorAttachment));
        GLCall(glDeleteTextures(1, &m_DepthAttachment));
    }

    GLCall(glGenFramebuffers(1, &m_RendererID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    GLCall(glGenTextures(1, &m_ColorAttachment));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

    GLCall(glGenTextures(1, &m_DepthAttachment));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0));

    CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::Bind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
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
