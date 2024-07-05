#pragma once

#include "Framebuffer.h"
#include "Core/Scene.h"
#include "Core/Texture.h"

class EnvCubemapFBO
{
public:
    EnvCubemapFBO(Scene* scene, const FramebufferSpecification& spec);
    ~EnvCubemapFBO();

    void Invalidate(Scene* scene);

    void RenderCubemap(Scene* scene);
    void RenderIrradianceMap(Scene* scene);
    void RenderPrefilterMap(Scene* scene);
    void RenderBRDFLutTexture(Scene* scene);
    void Bind();
    void Unbind();

    uint32_t GetID() const { return m_RendererID; }
    uint32_t GetEnvmapID() const { return m_EnvmapID; }
    uint32_t GetIrradianceMapID() const { return m_IrradianceMapID; }
    uint32_t GetPrefilterMapID() const { return m_PrefilterMapID; }
    Ref<TextureCubemap> GetEnvmap() const { return m_Envmap; }
    Ref<TextureCubemap> GetIrradianceMap() const { return m_IrradianceMap; }
    Ref<TextureCubemap> GetPrefilterMap() const { return m_PrefilterMap; }
    Ref<Texture2D> GetBRDFLutTexture() const { return m_BRDFLut_Texture; }
    const FramebufferSpecification& GetSpecification() const { return m_Specification; }

    static std::shared_ptr<EnvCubemapFBO> Create(Scene* scene, const FramebufferSpecification& spec = FramebufferSpecification());
private:
    void GenEnvBox();
    void GenIrradianceBox();
    void GenPrefilterBox();
    void GenBRDFLutQuad();
private:
    uint32_t m_RendererID;
    uint32_t m_EnvmapID, m_IrradianceMapID, m_PrefilterMapID, m_BRDFLutTexture, m_CaptureRBO;
    FramebufferSpecification m_Specification;
    Ref<TextureCubemap> m_Envmap, m_IrradianceMap, m_PrefilterMap;
    Ref<Texture2D> m_BRDFLut_Texture;
    Ref<Mesh> m_Mesh_EnvBox, m_Mesh_IrradBox, m_Mesh_PrefilterBox, m_Mesh_BRDFLutQuad;
    std::vector<glm::mat4> m_CaptureViews;
};