#pragma once

#include "pch.h"

#include <glm/glm.hpp>

struct FramebufferSpecification
{
    uint32_t Width, Height;

    uint32_t Samples = 1;

    bool SwapChainTarget = false;
    bool HDR = false;
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

    uint32_t GetID() const { return m_RendererID; }
    uint32_t* GetColorAttachments() { return m_ColorAttachments; }
    const FramebufferSpecification& GetSpecification() const { return m_Specification; }

    static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
private:
    void GenColorAttachment();
    void GenMultisamplesColorAttachment();
    void GenDepthAttachment();
private:
    uint32_t m_RendererID;
    uint32_t m_ColorAttachments[2];
    uint32_t m_DepthAttachment;
    FramebufferSpecification m_Specification;
};

class FramebufferManager
{
public:
    FramebufferManager() {};
    ~FramebufferManager() {};

    inline static void Create() { s_Instance = new FramebufferManager(); }
    static Ref<Framebuffer> CreateFramebuffer(std::string tag, const FramebufferSpecification& spec);
    static Ref<Framebuffer> GetByTag(const std::string& tag);
    static bool Remove(const std::string& tag);
public:
    static bool s_SMAA_Enabled;
private:
    static FramebufferManager* s_Instance;
    std::vector<std::pair<std::string, Ref<Framebuffer>>> m_Pools = {};
};
