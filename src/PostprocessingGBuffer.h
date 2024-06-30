#pragma once

#include "GBuffer.h"
#include "Core/Texture2D.h"
#include "Core/Material/ShaderMaterial.h"
#include "Core/Mesh.h"

class PostprocessingGBuffer
{
public:
    PostprocessingGBuffer();
    ~PostprocessingGBuffer() {};

    inline GBuffer* GetGBuffer() { return m_GBuffer.get(); }
    inline Mesh* GetPositionMesh() { return m_Mesh_Position.get(); }
    inline Mesh* GetNormalMesh() { return m_Mesh_Normal.get(); }
    inline Mesh* GetAlbedoMesh() { return m_Mesh_Albedo.get(); }
    inline Mesh* GetSpecularMesh() { return m_Mesh_Specular.get(); }
    inline Mesh* GetDeferredShadingMesh() { return m_Mesh_Deferred_Shading.get(); }

    void Bind();
    void Unbind();
private:
    Ref<GBuffer> m_GBuffer;
    Ref<Texture2D> m_Texture_Position, m_Texture_Normal, m_Texture_AlbedoSpec;
    Ref<ShaderMaterial> m_Material_Position, m_Material_Normal, m_Material_Albedo, m_Material_Specular, m_Material_Deferred_Shading;
    Ref<Mesh> m_Mesh_Position, m_Mesh_Normal, m_Mesh_Albedo, m_Mesh_Specular, m_Mesh_Deferred_Shading;
};
