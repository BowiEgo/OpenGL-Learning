#pragma once

#include "Core/Texture.h"

struct FBOTextureOptions {
    GLint minFilter = GL_NEAREST;
    GLint magFilter = GL_LINEAR;
    GLint wrapS = GL_CLAMP_TO_EDGE;
    GLint wrapT = GL_CLAMP_TO_EDGE;
    bool flip = true;

    uint32_t Width, Height;

    uint32_t Samples = 1;

    bool SwapChainTarget = false;
    bool HDR = false;
};

class FramebufferTexture : public Texture
{
public:
    FramebufferTexture(const FBOTextureOptions &options = FBOTextureOptions());
    ~FramebufferTexture();
private:
    FBOTextureOptions m_Options;
    uint32_t m_RendererID;
};

