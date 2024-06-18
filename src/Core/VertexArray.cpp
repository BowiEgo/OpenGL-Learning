#include "VertexArray.h"

#include "pch.h"

#include "Renderer.h"

#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferElement &element, const unsigned int stride)
{
    if (element.offset > 0)
        m_Offset = element.offset;

    GLCall(glVertexAttribPointer(m_Element_Count, element.count, element.type,
        element.normalized, stride, (const void*)(intptr_t)m_Offset));
    GLCall(glEnableVertexAttribArray(m_Element_Count));
    
    if (element.offset == 0)
        m_Offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
}

void VertexArray::AddBufferLayout(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        AddBuffer(vb, elements[i], layout.GetStride());
        m_Element_Count++;
    }

    m_Count = vb.GetCount() / m_Offset;
}

void VertexArray::IncreaseElementCount()
{
    m_Element_Count++;
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
