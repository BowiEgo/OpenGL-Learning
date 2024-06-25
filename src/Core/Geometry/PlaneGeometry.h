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
    PlaneGeometry(float* position[3]) {};
    ~PlaneGeometry() {};

    virtual inline std::vector<Vertex>* GetVertex() override { return &m_Vertex; }

    std::pair<glm::vec3, glm::vec3> CalcTangent(unsigned int triangleIdx);
private:
    std::vector<Vertex> m_Vertex = {
        {{  5.0f, -0.5f,  5.0f }, { 0.0f, 1.0f, 0.0f }, {  1.0f, 0.0f }, {}, {}, {}, {}},
        {{ -5.0f, -0.5f,  5.0f }, { 0.0f, 1.0f, 0.0f }, {  0.0f, 0.0f }, {}, {}, {}, {}},
        {{ -5.0f, -0.5f, -5.0f }, { 0.0f, 1.0f, 0.0f }, {  0.0f, 1.0f }, {}, {}, {}, {}},

        {{  5.0f, -0.5f,  5.0f }, { 0.0f, 1.0f, 0.0f }, {  1.0f, 0.0f }, {}, {}, {}, {}},
        {{ -5.0f, -0.5f, -5.0f }, { 0.0f, 1.0f, 0.0f }, {  0.0f, 1.0f }, {}, {}, {}, {}},
        {{  5.0f, -0.5f, -5.0f }, { 0.0f, 1.0f, 0.0f }, {  1.0f, 1.0f }, {}, {}, {}, {}},
    };
};