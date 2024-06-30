#pragma once

#include "pch.h"

struct GBufferSpecification
{
    uint32_t Width, Height;
};

class GBuffer
{
public:
    GBuffer(const GBufferSpecification& spec);
    ~GBuffer();

    static std::shared_ptr<GBuffer> Create(const GBufferSpecification& spec);
    void Invalidate();

    void Bind();
    void Unbind();

    uint32_t GetID() const { return m_RendererID; }
    uint32_t GetGPosition() const { return m_GPosition; }
    uint32_t GetGNormal() const { return m_GNormal; }
    uint32_t GetGAlbedoSpec() const { return m_GAlbedoSpec; }
    const GBufferSpecification& GetSpecification() const { return m_Specification; }
private:
    u_int32_t m_RendererID;
    u_int32_t m_GPosition, m_GNormal, m_GAlbedoSpec;
    u_int32_t m_Depth_Attachment;
    GBufferSpecification m_Specification;
};
