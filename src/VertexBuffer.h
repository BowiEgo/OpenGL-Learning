#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetID() const { return m_RendererID; }
	unsigned int GetCount() const { return m_Count; }
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};