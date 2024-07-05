#include "EnvCubemapFBO.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "FileSystem/FileSystem.h"

#include "Core/Texture2D.h"
#include "Core/TextureCubemap.h"
#include "Core/Geometry/BoxGeometry.h"
#include "Core/Geometry/QuadGeometry.h"

std::shared_ptr<EnvCubemapFBO> EnvCubemapFBO::Create(Scene* scene, const FramebufferSpecification &spec)
{
    GLCall(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
    return std::make_shared<EnvCubemapFBO>(scene, spec);
}

void EnvCubemapFBO::GenEnvBox()
{
    TextureOptions envOpts;
    envOpts.HDRI = true;
    Ref<Texture2D> hdr_map = std::make_shared<Texture2D>("../res/textures/HDR/newport_loft.hdr", envOpts);
    // preview cube
    Ref<BoxGeometry> geometry_previewBox = std::make_shared<BoxGeometry>();
    std::string envVertSrc = FileSystem::ReadFile("../res/shaders/CubemapSampler.vert");
    std::string envFragSrc = FileSystem::ReadFile("../res/shaders/CubemapSampler.frag");
    Ref<Shader> shader_previewBox = std::make_shared<Shader>(envVertSrc, envFragSrc);
    Ref<ShaderMaterial> material = std::make_shared<ShaderMaterial>(shader_previewBox);
    material->AddTexture("equirectangularMap", hdr_map);
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    m_CaptureViews = 
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    material->UpdateShaderUniform("projectionMatrix", captureProjection);
    m_Mesh_EnvBox = std::make_shared<Mesh>(geometry_previewBox, material);
}

void EnvCubemapFBO::GenIrradianceBox()
{
    std::string vertSrc = FileSystem::ReadFile("../res/shaders/CubemapSampler.vert");
    std::string fragSrc = FileSystem::ReadFile("../res/shaders/IrradianceConvolution.frag");
    Ref<Shader> shader_previewBox = std::make_shared<Shader>(vertSrc, fragSrc);
    Ref<ShaderMaterial> mateiral = std::make_shared<ShaderMaterial>(shader_previewBox);
    mateiral->AddTexture("environmentMap", m_Envmap);
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    mateiral->UpdateShaderUniform("projectionMatrix", captureProjection);
    m_Mesh_IrradBox = std::make_shared<Mesh>(std::make_shared<BoxGeometry>(), mateiral);
}

void EnvCubemapFBO::GenPrefilterBox()
{
    std::string vertSrc = FileSystem::ReadFile("../res/shaders/CubemapSampler.vert");
    std::string fragSrc = FileSystem::ReadFile("../res/shaders/Prefilter.frag");
    Ref<Shader> shader_previewBox = std::make_shared<Shader>(vertSrc, fragSrc);
    Ref<ShaderMaterial> mateiral = std::make_shared<ShaderMaterial>(shader_previewBox);
    mateiral->AddTexture("environmentMap", m_Envmap);
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    mateiral->UpdateShaderUniform("projectionMatrix", captureProjection);
    m_Mesh_PrefilterBox = std::make_shared<Mesh>(std::make_shared<BoxGeometry>(), mateiral);
}

void EnvCubemapFBO::GenBRDFLutQuad()
{
    std::string vertSrc = FileSystem::ReadFile("../res/shaders/BRDFLut.vert");
    std::string fragSrc = FileSystem::ReadFile("../res/shaders/BRDFLut.frag");
    Ref<Shader> shader = std::make_shared<Shader>(vertSrc, fragSrc);
    Ref<ShaderMaterial> mateiral = std::make_shared<ShaderMaterial>(shader);
    m_Mesh_BRDFLutQuad = std::make_shared<Mesh>(std::make_shared<QuadGeometry>(), mateiral);
}

EnvCubemapFBO::EnvCubemapFBO(Scene* scene, const FramebufferSpecification &spec)
    : m_Specification(spec)
{
    m_Specification.Width = 512;
    m_Specification.Height = 512;
    m_Specification.HDR = true;
    Invalidate(scene);
}

EnvCubemapFBO::~EnvCubemapFBO()
{
    GLCall(glDeleteFramebuffers(1, &m_RendererID));
    GLCall(glDeleteTextures(1, &m_EnvmapID));
    GLCall(glDeleteTextures(1, &m_IrradianceMapID));
    GLCall(glDeleteTextures(1, &m_PrefilterMapID));
    GLCall(glDeleteRenderbuffers(1, &m_CaptureRBO));
}

void EnvCubemapFBO::Invalidate(Scene* scene)
{
    if (m_RendererID)
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(1, &m_EnvmapID));
        GLCall(glDeleteTextures(1, &m_IrradianceMapID));
        GLCall(glDeleteTextures(1, &m_PrefilterMapID));
        GLCall(glDeleteRenderbuffers(1, &m_CaptureRBO));
    }

    GLCall(glGenFramebuffers(1, &m_RendererID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    // Envmap
    GLCall(glGenTextures(1, &m_EnvmapID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvmapID));
    for (unsigned int i = 0; i < 6; ++i)
    {
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_FLOAT, nullptr));
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); 
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // IrradianceMap
    GLCall(glGenTextures(1, &m_IrradianceMapID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMapID));
    for (unsigned int i = 0; i < 6; ++i)
    {
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr));
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);) 
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // PrefilterMap
    GLCall(glGenTextures(1, &m_PrefilterMapID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMapID));
    for (unsigned int i = 0; i < 6; ++i)
    {
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr));
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);) 
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

    // BrdfLUTTexture
    GLCall(glGenTextures(1, &m_BRDFLutTexture));
    // pre-allocate enough memory for the LUT texture.
    GLCall(glBindTexture(GL_TEXTURE_2D, m_BRDFLutTexture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // Create depth attachment
    GLCall(glGenRenderbuffers(1, &m_CaptureRBO));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Specification.Width, m_Specification.Height));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_CaptureRBO));

    CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    GenEnvBox();
    RenderCubemap(scene);
    GenIrradianceBox();
    RenderIrradianceMap(scene);
    GenPrefilterBox();
    RenderPrefilterMap(scene);
    GenBRDFLutQuad();
    RenderBRDFLutTexture(scene);
}

void EnvCubemapFBO::RenderCubemap(Scene* scene)
{
    Bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvmapID, 0));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_Mesh_EnvBox->GetMaterial()->UpdateShaderUniform("viewMatrix", m_CaptureViews[i]);
        scene->Draw(m_Mesh_EnvBox.get());
    }
    Unbind();

    m_Envmap = std::make_shared<TextureCubemap>("Texture_Environment", m_EnvmapID);
}

void EnvCubemapFBO::RenderIrradianceMap(Scene* scene)
{
    Bind();
    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    for (unsigned int i = 0; i < 6; ++i)
    {
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMapID, 0));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        m_Mesh_IrradBox->GetMaterial()->UpdateShaderUniform("viewMatrix", m_CaptureViews[i]);
        scene->Draw(m_Mesh_IrradBox.get());
    }
    Unbind();

    m_IrradianceMap = std::make_shared<TextureCubemap>("Texture_Environment", m_IrradianceMapID);
}

void EnvCubemapFBO::RenderPrefilterMap(Scene* scene)
{
    Bind();
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth  = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight));
        GLCall(glViewport(0, 0, mipWidth, mipHeight));

        float roughness = (float)mip / (float)(maxMipLevels - 1);

        m_Mesh_PrefilterBox->GetMaterial()->UpdateShaderUniform("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMapID, mip));

            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            m_Mesh_PrefilterBox->GetMaterial()->UpdateShaderUniform("viewMatrix", m_CaptureViews[i]);
            scene->Draw(m_Mesh_PrefilterBox.get());
        }
    }
    Unbind();

    m_PrefilterMap = std::make_shared<TextureCubemap>("Texture_Environment", m_PrefilterMapID);
}

void EnvCubemapFBO::RenderBRDFLutTexture(Scene *scene)
{
    Bind();
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BRDFLutTexture, 0));
    GLCall(glViewport(0, 0, 512, 512));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    scene->Draw(m_Mesh_BRDFLutQuad.get());
    Unbind();

    m_BRDFLut_Texture = std::make_shared<Texture2D>("Texture_Diffuse", m_BRDFLutTexture);
}

void EnvCubemapFBO::Bind()
{
    GLCall(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void EnvCubemapFBO::Unbind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
