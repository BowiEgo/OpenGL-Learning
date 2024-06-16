#pragma once

#include "pch.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"
#include "Geometry.h"

class QuadGeometry : public Geometry
{
public:
    QuadGeometry() {};
    QuadGeometry(float* position[3]) {};
    ~QuadGeometry() {};

    virtual inline std::vector<Vertex>* GetVertex() override { return &m_Vertex; }
private:
    std::vector<Vertex> m_Vertex = {
        // Front face
        {{ -1.0f,  1.0f, 0.0f }, {}, { 0.0f,  0.0f }, {}, {}, {}, {}},
        {{ -1.0f, -1.0f, 0.0f }, {}, { 0.0f,  1.0f }, {}, {}, {}, {}},
        {{  1.0f, -1.0f, 0.0f }, {}, { 1.0f,  1.0f }, {}, {}, {}, {}},

        {{ -1.0f,  1.0f, 0.0f }, {}, { 0.0f,  0.0f }, {}, {}, {}, {}},
        {{  1.0f, -1.0f, 0.0f }, {}, { 1.0f,  1.0f }, {}, {}, {}, {}},
        {{  1.0f,  1.0f, 0.0f }, {}, { 1.0f,  0.0f }, {}, {}, {}, {}},
    };
};