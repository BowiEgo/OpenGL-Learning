#pragma once

#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(float fov = 45.0f,
        float aspectRatio = 960.0f / 540.0f,
        const float position[3] = nullptr,
        const float target[3] = nullptr
    );
    ~Camera();

    inline float GetFOV() const { return m_FOV; }
    inline float GetAspectRatio() const { return m_AspectRatio; }
    inline glm::vec3 GetPosition() const { return m_CameraPos; }
    inline glm::vec3 GetTarget() const { return m_CameraTarget; }
    inline glm::vec3 GetFront() const { return m_CameraFront; }
    inline glm::vec3 GetRight() const { return m_CameraRight; }
    inline glm::vec3 GetUp() const { return m_CameraUp; }

    inline void SetFOV(float fov) { m_FOV = fov; }
    inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

    void ProcessKeyboardMovement(float deltaTime);
    void ProcessMouseMovement();
    void ProcessMouseScroll();
private:
    float m_FOV = 45.0f;
    float m_AspectRatio = 960.0f / 540.0f;
    glm::vec3 m_CameraPos;
    glm::vec3 m_CameraTarget;
    glm::vec3 m_CameraFront, m_CameraRight, m_CameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
    float m_Pitch = 0.0f, m_Yaw = 0.0f;
};