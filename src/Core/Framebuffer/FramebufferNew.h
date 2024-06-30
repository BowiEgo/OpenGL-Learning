#pragma once

#include "pch.h"

#include "Core/Texture/FramebufferTexture.h"

struct FramebufferSpecificationNew
{
    uint32_t Width, Height;
};

class FramebufferNew
{
public:
    FramebufferNew(const FramebufferSpecificationNew& spec);
    ~FramebufferNew();

    void AttachColorTexture(Ref<FramebufferTexture> texture);

    void Bind();
    void Unbind();
private:
    uint32_t m_RendererID;
    FramebufferSpecificationNew m_Specification;

    std::vector<Ref<FramebufferTexture>> m_ColorAttachments;
};
