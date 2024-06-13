#pragma once

#include "pch.h"
#include "Core/Renderer.h"

struct TextureCubemapOptions {
    GLint wrapS = GL_CLAMP_TO_EDGE;
    GLint wrapT = GL_CLAMP_TO_EDGE;
    GLint wrapR = GL_CLAMP_TO_EDGE;
    GLint minFilter = GL_LINEAR;
    GLint magFilter = GL_LINEAR;
    bool flip = false;
};

class TextureCubemap
{
public:
    TextureCubemap(const std::string& type);
    TextureCubemap(const std::string& type, const unsigned int rendererID);
    TextureCubemap(const std::string& type, const std::vector<std::string> facePaths, const TextureCubemapOptions& options = TextureCubemapOptions());
    ~TextureCubemap();

    void SetupTextureCubemap(const std::vector<std::string> facePaths, const TextureCubemapOptions& options = TextureCubemapOptions());
    void SetupEmptyTextureCubemap();

    static Ref<TextureCubemap> CreateEmptyTexture(const std::string& type, const TextureCubemapOptions& options = TextureCubemapOptions());

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    void SetWrapping(GLenum pname, GLint param) const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
    inline unsigned int GetID() const { return m_RendererID; }
    inline std::string GetType() const { return m_Type; }
    inline std::vector<std::string> GetFilePaths() const { return m_FilePaths; }
private:
    TextureCubemapOptions m_Options;
    unsigned int m_RendererID;
    std::string m_Type = "";
    std::vector<std::string> m_FilePaths;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_Channels;
};
