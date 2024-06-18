#pragma once

#include "VertexBuffer.h"

struct VertexBufferLayout;
struct VertexBufferElement;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	unsigned int GetID() const { return m_RendererID; };

	void AddBuffer(const VertexBuffer &vb, const VertexBufferElement &element, const unsigned int stride);
	void AddBufferLayout(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void IncreaseElementCount();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return m_Count; }
	unsigned int GetElementCount() const { return m_Element_Count; }
private:
	unsigned int m_RendererID;
	unsigned int m_Count = 0;
	unsigned int m_Element_Count = 0;
	unsigned int m_Offset = 0;
};
