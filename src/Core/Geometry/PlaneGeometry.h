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

    virtual std::shared_ptr<VertexArray> GetVAO() const override { return m_VAO; }
    inline std::array<Vertex, 36>* GetVertex() override { return &m_Vertex; }
private:
    std::shared_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;

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