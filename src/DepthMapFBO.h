#pragma once

#include "Framebuffer.h"

class DepthMapFBO
{
public:
    DepthMapFBO(const FramebufferSpecification& spec);
    ~DepthMapFBO();

    void Invalidate();

    void Bind();
    void Unbind();

    uint32_t GetID() const { return m_RendererID; }
    uint32_t GetDepthMapID() const { return m_DepthMap; }
    const FramebufferSpecification& GetSpecification() const { return m_Specification; }

    static std::shared_ptr<DepthMapFBO> Create(const FramebufferSpecification& spec);
private:
    uint32_t m_RendererID;
    uint32_t m_DepthMap;
    FramebufferSpecification m_Specification;
};