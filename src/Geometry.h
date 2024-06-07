#pragma once

#include "Vertex.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <memory>

class Geometry
{
public:
	Geometry() {};
	virtual ~Geometry() {};

    virtual std::shared_ptr<VertexArray> GetVAO() const = 0;
    virtual std::array<Vertex, 36>* GetVertex() = 0;
private:
	std::shared_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
};