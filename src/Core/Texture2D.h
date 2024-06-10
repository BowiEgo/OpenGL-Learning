#pragma once

#include "pch.h"
#include "Core/Renderer.h"

struct TextureOptions {
    GLint wrapS = GL_CLAMP_TO_EDGE;
    GLint wrapT = GL_CLAMP_TO_EDGE;
    GLint minFilter = GL_NEAREST;
    GLint magFilter = GL_LINEAR;
};

class Texture2D
{
public:
    Texture2D(const std::string& path, const TextureOptions& options = TextureOptions());
    Texture2D(const std::string& type, const std::string& path, const TextureOptions& options = TextureOptions());
    ~Texture2D();

    void SetupTexture2D(const std::string& type, const std::string& path, const TextureOptions& options = TextureOptions());

    static unsigned int Create(const std::string& type, const std::string& path, const TextureOptions& options = TextureOptions());

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    void SetWrapping(GLenum pname, GLint param) const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
    inline unsigned int GetID() const { return m_RendererID; }
    inline std::string GetType() const { return m_Type; }
    inline std::string GetFilePath() const { return m_FilePath; }
private:
    TextureOptions m_Options;
    unsigned int m_RendererID;
    std::string m_Type = "";
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_Channels;
};
