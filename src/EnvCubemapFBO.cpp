#include "EnvCubemapFBO.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "FileSystem/FileSystem.h"

#include "Core/Texture2D.h"
#include "Core/TextureCubemap.h"
#include "Core/Geometry/BoxGeometry.h"

std::shared_ptr<EnvCubemapFBO> EnvCubemapFBO::Create(const FramebufferSpecification &spec)
{
    return std::make_shared<EnvCubemapFBO>(spec);
}

void EnvCubemapFBO::GenEnvBox()
{
    TextureOptions envOpts;
    envOpts.HDR = true;
    Ref<Texture2D> env_map = std::make_shared<Texture2D>("../res/textures/HDR/newport_loft.hdr", envOpts);
    // preview cube
    Ref<BoxGeometry> geometry_previewBox = std::make_shared<BoxGeometry>();
    std::string envVertSrc = FileSystem::ReadFile("../res/shaders/CubemapSampler.vert");
    std::string envFragSrc = FileSystem::ReadFile("../res/shaders/CubemapSampler.frag");
    Ref<Shader> shader_previewBox = std::make_shared<Shader>(envVertSrc, envFragSrc);
    Ref<ShaderMaterial> material = std::make_shared<ShaderMaterial>(shader_previewBox);
    // Ref<BasicMaterial> material = std::make_shared<BasicMaterial>();
    material->AddTexture("equirectangularMap", env_map);
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
    Ref<TextureCubemap> envmap_texture = std::make_shared<TextureCubemap>("Texture_Environment", m_Envmap);
    std::string irradVertSrc = FileSystem::ReadFile("../res/shaders/CubemapSampler.vert");
    std::string irradFragSrc = FileSystem::ReadFile("../res/shaders/IrradianceConvolution.frag");
    Ref<Shader> shader_previewBox = std::make_shared<Shader>(irradVertSrc, irradFragSrc);
    Ref<ShaderMaterial> mateiral = std::make_shared<ShaderMaterial>(shader_previewBox);
    // Ref<BasicMaterial> mateiral = std::make_shared<BasicMaterial>();
    mateiral->AddTexture("environmentMap", envmap_texture);
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    mateiral->UpdateShaderUniform("projectionMatrix", captureProjection);
    m_Mesh_IrradBox = std::make_shared<Mesh>(std::make_shared<BoxGeometry>(), mateiral);
}

EnvCubemapFBO::EnvCubemapFBO(const FramebufferSpecification &spec)
    : m_Specification(spec)
{
    m_Specification.Width = 512;
    m_Specification.Height = 512;
    m_Specification.HDR = true;
    Invalidate();
}

EnvCubemapFBO::~EnvCubemapFBO()
{
    GLCall(glDeleteFramebuffers(1, &m_RendererID));
    GLCall(glDeleteTextures(1, &m_Envmap));
    GLCall(glDeleteRenderbuffers(1, &m_DepthAttachment));
}

void EnvCubemapFBO::Invalidate()
{
    if (m_RendererID)
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(1, &m_Envmap));
        GLCall(glDeleteRenderbuffers(1, &m_DepthAttachment));
    }

    GLCall(glGenFramebuffers(1, &m_RendererID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    // Envmap
    GLCall(glGenTextures(1, &m_Envmap));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Envmap));
    for (unsigned int i = 0; i < 6; ++i)
    {
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_FLOAT, nullptr));
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);) 
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // IrradianceMap
    GLCall(glGenTextures(1, &m_IrradianceMap));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap));
    for (unsigned int i = 0; i < 6; ++i)
    {
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr));
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);) 
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // Create depth attachment
    GLCall(glGenRenderbuffers(1, &m_DepthAttachment));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Specification.Width, m_Specification.Height));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment));

    CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    GenEnvBox();
    GenIrradianceBox();
}

void EnvCubemapFBO::RenderCubemap(Scene* scene)
{
    Bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_Envmap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Mesh_EnvBox->GetMaterial()->UpdateShaderUniform("viewMatrix", m_CaptureViews[i]);
        scene->Draw(m_Mesh_EnvBox.get());
    }
    Unbind();
}

void EnvCubemapFBO::RenderIrradianceMap(Scene *scene)
{
    Bind();
    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    for (unsigned int i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Mesh_IrradBox->GetMaterial()->UpdateShaderUniform("viewMatrix", m_CaptureViews[i]);
        scene->Draw(m_Mesh_IrradBox.get());
    }
    Unbind();
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
