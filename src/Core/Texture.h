#pragma once

#include "pch.h"
#include "Core/Renderer.h"

class Texture
{
public:
    virtual ~Texture() {};

    virtual void Bind(unsigned int slot = 0) const = 0;
    virtual void Unbind() const = 0;

    virtual void SetWrapping(GLenum pname, GLint param) const = 0;

    virtual inline int GetWidth() const = 0;
    virtual inline int GetHeight() const = 0;
    virtual inline unsigned int GetID() const = 0;
    virtual inline std::string GetType() const = 0;
private:
};
