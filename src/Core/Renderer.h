#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>

#include "Log.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Core/Shader.h"

class Renderer
{
private:
    /* data */
public:
    void Clear() const;
    void Draw(const Shader& shader, const VertexArray& va) const;
    void Draw(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const;
};
