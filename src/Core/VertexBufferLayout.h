#pragma once

#include "pch.h"

#include "Renderer.h"
#include "Geometry/Geometry.h"
#include "Mesh.h"

struct VertexBufferElement
{
    /* data */
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    unsigned int offset;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:         return 4;
            case GL_UNSIGNED_INT:  return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        ASSERT(false);
        return 0;
    }
};


struct VertexBufferLayout
{
    /* data */
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout()
        : m_Stride(0) {};

    template<typename T>
    void Push(unsigned int count)
    {
        static_assert(sizeof(T) == 0, "Unsupported type for VertexBufferLayout::Push");
    }

    template<typename T>
    void Push(unsigned int count, unsigned int offset)
    {
        static_assert(sizeof(T) == 0, "Unsupported type for VertexBufferLayout::Push");
    }

    template<>
    void Push<float>(unsigned int count)
    {
        m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    template<>
    void Push<Vertex>(unsigned int count, unsigned int offset)
    {
        m_Elements.push_back({GL_FLOAT, count, GL_FALSE, offset});
        m_Stride = (unsigned int)sizeof(Vertex);
    }

    inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};
