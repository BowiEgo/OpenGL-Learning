#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(
        const float position[3] = nullptr,
        const float target[3] = nullptr,
        float fov = 45.0f,
        float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT,
        float near = 0.1f,
        float far = 100.0f
    );
    PerspectiveCamera(
        const glm::vec3 position,
        const glm::vec3 target,
        float fov = 45.0f,
        float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT,
        float near = 0.1f,
        float far = 100.0f
    );
    ~PerspectiveCamera();

    virtual inline CameraType GetType() const override { return m_Type; };

    inline float GetFOV() const { return m_FOV; }
    inline float GetAspectRatio() const { return m_AspectRatio; }

    void SetFOV(const float& fov);
    void SetAspectRatio(const float& aspectRatio);
private:
    virtual void UpdateProjMatrix() override;
private:
    CameraType m_Type = CameraType::Perspective;
    float m_FOV;
    float m_AspectRatio;
};