#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(
        const float position[3] = nullptr,
        const float target[3] = nullptr,
        float near = 1.0f,
        float far = 7.5f
    );
    ~OrthographicCamera();

    virtual inline CameraType GetType() const override { return m_Type; };
private:
    virtual void UpdateProjMatrix() override;
private:
    CameraType m_Type = CameraType::Orthographic;
    float m_ZoomLevel = 1.0f;
    float m_Near, m_Far;
};