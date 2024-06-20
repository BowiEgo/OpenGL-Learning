#pragma once

#include "pch.h"

#include <glm/glm.hpp>

#include "Camera/Camera.h"
#include "Core/Shader/Shader.h"

class Light
{
public:
    virtual ~Light() = default;

    virtual void SetID(unsigned int id) = 0;
    virtual void Update(Shader* shader) = 0;
private:
};