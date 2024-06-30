#include "PostprocessingGBuffer.h"

#include "Core/Geometry/QuadGeometry.h"
#include "FileSystem/FileSystem.h"

PostprocessingGBuffer::PostprocessingGBuffer()
{
    GBufferSpecification fbSpec;
    fbSpec.Width = WINDOW_WIDTH;
    fbSpec.Height = WINDOW_HEIGHT;
    m_GBuffer = GBuffer::Create(fbSpec);
    // textures
    uint32_t texture_position = m_GBuffer->GetGPosition();
    m_Texture_Position = std::make_shared<Texture2D>("Texture_Diffuse", texture_position);

    uint32_t texture_normal = m_GBuffer->GetGNormal();
    m_Texture_Normal = std::make_shared<Texture2D>("Texture_Diffuse", texture_normal);

    uint32_t texture_albedoSpec = m_GBuffer->GetGAlbedoSpec();
    m_Texture_AlbedoSpec = std::make_shared<Texture2D>("Texture_Diffuse", texture_albedoSpec);
    // shader
    std::string normalizedVertSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 a_Position;
        layout (location = 1) in vec3 a_Normal;
        layout (location = 2) in vec2 a_TexCoords;

        uniform int u_Normalized_Index;

        out vec2 v_TexCoords;

        void main()
        {
            vec2 final_pos;
            float screenNr = 4;
            if (u_Normalized_Index == 0)
                final_pos = vec2(a_Position.x - 3.0, - a_Position.y + 3.0);

            if (u_Normalized_Index == 1)
                final_pos = vec2(a_Position.x - 3.0, - a_Position.y + 1.0);

            if (u_Normalized_Index == 2)
                final_pos = vec2(a_Position.x - 3.0, - a_Position.y - 1.0);

            if (u_Normalized_Index == 3)
                final_pos = vec2(a_Position.x - 3.0, - a_Position.y - 3.0);

            if (u_Normalized_Index == 4)
                final_pos = vec2(a_Position.x, -a_Position.y);

            gl_Position = vec4(final_pos, 0.0, screenNr);

            if (u_Normalized_Index == 4)
                gl_Position = vec4(final_pos, 0.0, 1.0);

            v_TexCoords = a_TexCoords;
        }
    )";
    std::string normalizedFragSrc = R"(
        #version 330 core
        layout (location = 0) out vec4 FragColor;
        
        in vec2 v_TexCoords;

        uniform sampler2D u_Texture_Diffuse1;

        uniform int u_Normalized_Index;

        void main()
        {
            vec4 screenTexture = texture(u_Texture_Diffuse1, v_TexCoords);

            FragColor = vec4(screenTexture.rgb, 1.0);
            if (u_Normalized_Index == 3)
                FragColor = vec4(vec3(1.0 - screenTexture.a), 1.0);
        }
    )";
    // materials
    m_Material_Position = std::make_shared<ShaderMaterial>(std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc));
    m_Material_Position->UpdateShaderUniform("u_Normalized_Index", 0);
    m_Material_Position->AddTexture(m_Texture_Position);

    m_Material_Normal = std::make_shared<ShaderMaterial>(std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc));
    m_Material_Normal->UpdateShaderUniform("u_Normalized_Index", 1);
    m_Material_Normal->AddTexture(m_Texture_Normal);

    m_Material_Albedo = std::make_shared<ShaderMaterial>(std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc));
    m_Material_Albedo->UpdateShaderUniform("u_Normalized_Index", 2);
    m_Material_Albedo->AddTexture(m_Texture_AlbedoSpec);

    m_Material_Specular = std::make_shared<ShaderMaterial>(std::make_shared<Shader>(normalizedVertSrc, normalizedFragSrc));
    m_Material_Specular->UpdateShaderUniform("u_Normalized_Index", 3);
    m_Material_Specular->AddTexture(m_Texture_AlbedoSpec);

    std::string deferred_shading_fragSrc = FileSystem::ReadFile("../res/shaders/DeferredShading.frag");
    m_Material_Deferred_Shading = std::make_shared<ShaderMaterial>(std::make_shared<Shader>(normalizedVertSrc, deferred_shading_fragSrc));
    m_Material_Deferred_Shading->UpdateShaderUniform("u_Normalized_Index", 4);
    m_Material_Deferred_Shading->AddTexture("g_Position", m_Texture_Position);
    m_Material_Deferred_Shading->AddTexture("g_Normal", m_Texture_Normal);
    m_Material_Deferred_Shading->AddTexture("g_AlbedoSpec", m_Texture_AlbedoSpec);
    // mesh
    m_Mesh_Position = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), m_Material_Position);
    m_Mesh_Normal = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), m_Material_Normal);
    m_Mesh_Albedo = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), m_Material_Albedo);
    m_Mesh_Specular = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), m_Material_Specular);
    m_Mesh_Deferred_Shading = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), m_Material_Deferred_Shading);
    m_Mesh_Deferred_Shading->DepthTest_Enabled = false;
}

void PostprocessingGBuffer::Bind()
{
    m_GBuffer->Bind();
}

void PostprocessingGBuffer::Unbind()
{
    m_GBuffer->Unbind();
}
