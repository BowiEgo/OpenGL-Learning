#include "SphereGeometry.h"

SphereGeometry::SphereGeometry(float radius, unsigned int widthSegments, unsigned int heightSegments, float phiStart, float phiLength, float thetaStart, float thetaLength)
{
    unsigned int width_segments = std::max(3u, widthSegments);
    unsigned int height_segments = std::max(2u, heightSegments);

    float thetaEnd = std::min(thetaStart + thetaLength, PI);

    unsigned int index = 0;
    std::vector<std::vector<float>> grid;

    for (unsigned int iy = 0; iy <= height_segments; iy++) {
        std::vector<float> verticesRow;
        float v = static_cast<float>(iy) / height_segments;
        float uOffset = 0;
        if (iy == 0 && thetaStart == 0) {
            uOffset = 0.5 / width_segments;
        } else if (iy == height_segments && thetaEnd == PI) {
            uOffset = -0.5 / width_segments;
        }

        for (unsigned int ix = 0; ix <= width_segments; ix++) {
            float u = static_cast<float>(ix) / width_segments;

            float xPos = - radius * std::cos(phiStart   + u * phiLength) * std::sin(thetaStart + v * thetaLength);
            float yPos =   radius * std::cos(thetaStart + v * thetaLength);
            float zPos =   radius * std::sin(phiStart   + u * phiLength) * std::sin(thetaStart + v * thetaLength);

            m_Vertex.push_back({{xPos, yPos, zPos}, {xPos, yPos, zPos}, {u + uOffset, 1 - v}, {}, {}, {}, {}});

            verticesRow.push_back(index++);
        }

        grid.push_back(verticesRow);
    }

    for (unsigned int iy = 0; iy < height_segments; iy++ ) {
        for (unsigned int ix = 0; ix < width_segments; ix++) {
            unsigned int a = grid[iy][ix + 1];
            unsigned int b = grid[iy][ix];
            unsigned int c = grid[iy + 1][ix];
            unsigned int d = grid[iy + 1][ix + 1];

            if (iy != 0 || thetaStart > 0)
                m_Indices.insert(m_Indices.end(), {a, b, d});

            if (iy != height_segments - 1 || thetaEnd < PI)
                m_Indices.insert(m_Indices.end(), {b, c, d});
        }
    }
}   
