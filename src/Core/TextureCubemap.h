#pragma once

#include "pch.h"
#include "Core/Renderer.h"

#include "Texture.h"

struct TextureCubemapOptions {
    GLint wrapS = GL_CLAMP_TO_EDGE;
    GLint wrapT = GL_CLAMP_TO_EDGE;
    GLint wrapR = GL_CLAMP_TO_EDGE;
    GLint minFilter = GL_LINEAR;
    GLint magFilter = GL_LINEAR;
    bool flip = false;
};

class TextureCubemap : public Texture
{
public:
    TextureCubemap(const std::string& type, const unsigned int rendererID);
    TextureCubemap(const std::string& type, int width, int height, const TextureCubemapOptions& options = TextureCubemapOptions());
    TextureCubemap(const std::string& type, const std::vector<std::string> facePaths, const TextureCubemapOptions& options = TextureCubemapOptions());
    ~TextureCubemap();

    void SetupTextureCubemap(const std::vector<std::string> facePaths, const TextureCubemapOptions& options = TextureCubemapOptions());
    void SetupDepthCubemap(const TextureCubemapOptions& options = TextureCubemapOptions());

    static TextureCubemap* CreateVoidTexture(const std::string& type);

    virtual void Bind(unsigned int slot = 0) const override;
    virtual void Unbind() const override;

    virtual void SetWrapping(GLenum pname, GLint param) const override;

    virtual inline int GetWidth() const override { return m_Width; }
    virtual inline int GetHeight() const override { return m_Height; }
    virtual inline unsigned int GetID() const override { return m_RendererID; }
    virtual inline std::string GetType() const override { return m_Type; }
    inline std::vector<std::string> GetFilePaths() const { return m_FilePaths; }
private:
    TextureCubemapOptions m_Options;
    unsigned int m_RendererID;
    std::string m_Type = "";
    std::vector<std::string> m_FilePaths;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_Channels;
};
