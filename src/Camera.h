#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <initializer_list>
#include "Log.h"

class Camera
{
public:
    Camera(
        const float position[3] = nullptr,
        const float target[3] = nullptr,
        float fov = 45.0f,
        float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT,
        float near = 0.1f,
        float far = 100.0f
    );
    ~Camera();

    inline float GetFOV() const { return m_FOV; }
    inline float GetAspectRatio() const { return m_AspectRatio; }
    inline glm::vec3 GetPosition() const { return m_CameraPos; }
    inline glm::vec3 GetTarget() const { return m_CameraTarget; }
    inline glm::vec3 GetFront() const { return m_CameraFront; }
    inline glm::vec3 GetRight() const { return m_CameraRight; }
    inline glm::vec3 GetUp() const { return m_CameraUp; }
    inline glm::mat4 GetProjMatrix() const { return m_ProjMatrix; }
    inline glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

    void SetFOV(const float& fov);
    void SetAspectRatio(const float& aspectRatio);
    void SetPosition(const float position[3]);
    void SetPosition(const glm::vec3& position);
    void SetPositionX(const float& x);
    void SetPositionY(const float& y);
    void SetPositionZ(const float& z);
    void SetTarget(const float target[3]);
    void SetTarget(const glm::vec3& target);

    void ProcessKeyboardMovement(const float& deltaTime);
    void ProcessMouseMovement();
    void ProcessMouseScroll();

    void EnableControll();
    void DisableControll();
private:
    void UpdateProjMatrix();
    void UpdateCameraFront(const glm::vec3& direction);
    void UpdatePitchAndYaw();
    float GetYawByDirection(const glm::vec3& direction);
    float GetPitchByDirection(const glm::vec3& direction);
private:
    float m_FOV;
    float m_ZoomLevel = 1.0f;
    float m_AspectRatio;
    float m_Near, m_Far;

    glm::mat4 m_ProjMatrix, m_ViewMatrix;

    glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_CameraTarget = glm::vec3(0.0f, 0.0f ,0.0f);
    glm::vec3 m_CameraFront,
        m_CameraRight,
        m_CameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    float m_Pitch = 0.0f, m_Yaw = -90.0f;
    float m_LastX = 0.0f, m_LastY = 0.0f, m_LastScroll = 0.0f;
    bool m_FirstMouse = true;
    float m_MoveSpeed = 5.0f, m_PitchSpeed = 0.1f, m_YawSpeed = 0.1f;

    bool m_Controll_Enabled = true;
};