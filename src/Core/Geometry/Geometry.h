#pragma once

#include "pch.h"

#include "Core/Vertex.h"
#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"

class Geometry
{
public:
	Geometry() {};
	virtual ~Geometry() {};

    virtual std::vector<Vertex>* GetVertex() = 0;
	virtual std::vector<unsigned int>* GetIndices() = 0;
};