#pragma once

#include "pch.h"

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"
#include "Geometry.h"

class SphereGeometry : public Geometry
{
public:
    SphereGeometry(float radius = 1.0f, unsigned int widthSegments = 32, unsigned int heightSegments = 16, float phiStart = 0, float phiLength = PI * 2, float thetaStart = 0, float thetaLength = PI);
    ~SphereGeometry() {};

    virtual inline std::vector<Vertex>* GetVertex() override { return &m_Vertex; }
	virtual inline std::vector<unsigned int>* GetIndices() override { return &m_Indices; }
private:
    std::vector<Vertex> m_Vertex;
    std::vector<unsigned int> m_Indices;
};