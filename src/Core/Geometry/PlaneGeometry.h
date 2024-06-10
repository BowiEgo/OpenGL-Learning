#pragma once

#include "pch.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"
#include "Geometry.h"

class PlaneGeometry : public Geometry
{
public:
    PlaneGeometry();
    PlaneGeometry(float* position[3]);
    ~PlaneGeometry();

    inline std::array<Vertex, 36>* GetVertex() override { return &m_Vertex; }
private:
    std::array<Vertex, 36> m_Vertex = {{
        // Front face
        {{  5.0f, -0.5f,  5.0f }, {}, { 2.0f, 0.0f }, {}, {}, {}, {}},
        {{ -5.0f, -0.5f,  5.0f }, {}, { 0.0f, 0.0f }, {}, {}, {}, {}},
        {{ -5.0f, -0.5f, -5.0f }, {}, { 0.0f, 2.0f }, {}, {}, {}, {}},

        {{  5.0f, -0.5f,  5.0f }, {}, { 2.0f, 0.0f }, {}, {}, {}, {}},
        {{ -5.0f, -0.5f, -5.0f }, {}, { 0.0f, 2.0f }, {}, {}, {}, {}},
        {{  5.0f, -0.5f, -5.0f }, {}, { 2.0f, 2.0f }, {}, {}, {}, {}},
    }};
};