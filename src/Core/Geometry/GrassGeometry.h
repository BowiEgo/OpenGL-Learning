#pragma once

#include "pch.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/Texture2D.h"
#include "Geometry.h"

class GrassGeometry : public Geometry
{
public:
    GrassGeometry() {};
    GrassGeometry(float* position[3]) {};
    ~GrassGeometry() {};

    virtual inline std::vector<Vertex>* GetVertex() override { return &m_Vertex; }
private:
    std::vector<Vertex> m_Vertex = {
        // Front face
        {{  0.0f,  0.5f,  0.0f }, {}, { 0.0f,  0.0f }, {}, {}, {}, {}},
        {{  0.0f, -0.5f,  0.0f }, {}, { 0.0f,  1.0f }, {}, {}, {}, {}},
        {{  1.0f, -0.5f,  0.0f }, {}, { 1.0f,  1.0f }, {}, {}, {}, {}},

        {{  0.0f,  0.5f,  0.0f }, {}, { 0.0f,  0.0f }, {}, {}, {}, {}},
        {{  1.0f, -0.5f,  0.0f }, {}, { 1.0f,  1.0f }, {}, {}, {}, {}},
        {{  1.0f,  0.5f,  0.0f }, {}, { 1.0f,  0.0f }, {}, {}, {}, {}},
    };
};