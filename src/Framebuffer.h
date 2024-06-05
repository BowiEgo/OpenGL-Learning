#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>

struct FramebufferSpecification
{
    uint32_t Width, Height;

    uint32_t Samples = 1;

    bool SwapChainTarget = false;
};

class Framebuffer
{
public:
    Framebuffer(const FramebufferSpecification& spec);
    ~Framebuffer();

    void Invalidate();

    void Bind();
    void Unbind();

    void Resize(uint32_t width, uint32_t height);

    uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
    const FramebufferSpecification& GetSpecification() const { return m_Specification; }

    static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
private:
    uint32_t m_RendererID;
    uint32_t m_ColorAttachment, m_DepthAttachment;
    FramebufferSpecification m_Specification;
};