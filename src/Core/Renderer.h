#pragma once

#include <glad/glad.h>

#include "pch.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Core/Shader/Shader.h"

enum class DrawWay
{
    None = 0, Instanced
};

enum class DrawType
{
    Traingles = 0, Points, Lines
};

class Renderer
{
private:
    /* data */
public:
    void Clear() const;
    void Draw(const VertexArray& va, const DrawWay& drawWay, const DrawType& drawType, const unsigned int& instancecount = 0) const;
    void Draw(const VertexArray &va, const IndexBuffer& ib, const DrawWay& drawWay, const DrawType& drawType, const unsigned int& instancecount = 0) const;
    void Draw(const Shader& shader, const VertexArray& va) const;
    void Draw(const Shader& shader, const VertexArray& va, const IndexBuffer& ib) const;
};
