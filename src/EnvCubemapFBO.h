#pragma once

#include "Framebuffer.h"
#include "Core/Scene.h"
#include "Core/Texture.h"

class EnvCubemapFBO
{
public:
    EnvCubemapFBO(const FramebufferSpecification& spec);
    ~EnvCubemapFBO();

    void Invalidate();

    void RenderCubemap(Scene* scene);
    void RenderIrradianceMap(Scene* scene);
    void Bind();
    void Unbind();

    uint32_t GetID() const { return m_RendererID; }
    uint32_t GetEnvmapID() const { return m_Envmap; }
    uint32_t GetIrradianceMapID() const { return m_IrradianceMap; }
    uint32_t GetDepthMapID() const { return m_DepthAttachment; }
    const FramebufferSpecification& GetSpecification() const { return m_Specification; }

    static std::shared_ptr<EnvCubemapFBO> Create(const FramebufferSpecification& spec = FramebufferSpecification());
private:
    void GenEnvBox();
    void GenIrradianceBox();
private:
    uint32_t m_RendererID;
    uint32_t m_Envmap, m_DepthAttachment, m_IrradianceMap;
    FramebufferSpecification m_Specification;
    Ref<Mesh> m_Mesh_EnvBox, m_Mesh_IrradBox;
    std::vector<glm::mat4> m_CaptureViews;
};