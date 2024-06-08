#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "Camera.h"
#include "Core/Shader.h"

class Light
{
public:
    virtual ~Light() = default;

    virtual void SetID(unsigned int id) = 0;
    virtual void Update(Shader* shader) = 0;
private:
};