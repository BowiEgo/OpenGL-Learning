#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	unsigned int GetID() const { return m_RendererID; };

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return m_Count; }
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};
