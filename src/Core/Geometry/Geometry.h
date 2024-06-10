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

    virtual std::array<Vertex, 36>* GetVertex() = 0;
};