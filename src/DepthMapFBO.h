#pragma once

#include "Framebuffer.h"
#include "./Core/Texture.h"

class DepthMapFBO
{
public:
    DepthMapFBO(const std::string textureType, const FramebufferSpecification& spec);
    ~DepthMapFBO();

    void Invalidate();

    void Bind();
    void Unbind();

    uint32_t GetID() const { return m_RendererID; }
    uint32_t GetColorAttachmentID() const { return m_ColorAttachment; }
    uint32_t GetDepthMapID() const { return m_DepthAttachment; }
    const FramebufferSpecification& GetSpecification() const { return m_Specification; }

    static std::shared_ptr<DepthMapFBO> Create(const FramebufferSpecification& spec = FramebufferSpecification());
    static std::shared_ptr<DepthMapFBO> Create(const std::string textureType, const FramebufferSpecification& spec = FramebufferSpecification());
private:
    uint32_t m_RendererID;
    uint32_t m_ColorAttachment, m_DepthAttachment;
    std::string m_TextureType;
    FramebufferSpecification m_Specification;
};