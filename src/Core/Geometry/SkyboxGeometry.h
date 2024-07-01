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

class SkyboxGeometry : public Geometry
{
public:
    SkyboxGeometry() {};
    SkyboxGeometry(float* position[3]) {};
    ~SkyboxGeometry() {};

    virtual inline std::vector<Vertex>* GetVertex() override { return &m_Vertex; }
	virtual inline std::vector<unsigned int>* GetIndices() override { return &m_Indices; }
private:
    std::vector<unsigned int> m_Indices;
    std::vector<Vertex> m_Vertex = {
        // Back face
        {{-1.0f,  1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f, -1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f, -1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f, -1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f,  1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f,  1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},

        // Front face
        {{-1.0f, -1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f, -1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f,  1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f,  1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f,  1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f, -1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},

        // Left face
        {{ 1.0f, -1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f, -1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f,  1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f,  1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f,  1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f, -1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},

        // Right face
        {{-1.0f, -1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f,  1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f,  1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f,  1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f, -1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f, -1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},

        // Bottom face
        {{-1.0f,  1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f,  1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f,  1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f,  1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f,  1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f,  1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},

        // Top face
        {{-1.0f, -1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f, -1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f, -1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f, -1.0f, -1.0f}, {}, {}, {}, {}, {}, {}},
        {{-1.0f, -1.0f,  1.0f}, {}, {}, {}, {}, {}, {}},
        {{ 1.0f, -1.0f,  1.0f}, {}, {}, {}, {}, {}, {}}
    };
};
    