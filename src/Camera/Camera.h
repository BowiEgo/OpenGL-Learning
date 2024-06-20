#pragma once

#include <glm/gtc/matrix_transform.hpp>

enum CameraType {
    Perspective,
    Orthographic
};

class Camera
{
public:
    ~Camera();

    virtual inline CameraType GetType() const = 0;
    inline glm::vec3 GetPosition() const { return m_CameraPos; }
    inline glm::vec3 GetTarget() const { return m_CameraTarget; }
    inline glm::vec3 GetFront() const { return m_CameraFront; }
    inline glm::vec3 GetRight() const { return m_CameraRight; }
    inline glm::vec3 GetUp() const { return m_CameraUp; }
    inline glm::mat4 GetProjMatrix() const { return m_ProjMatrix; }
    inline glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

    void Init();
    void SetZoomLevel(const float& zoomlv);
    void SetNear(const float& near);
    void SetFar(const float& far);
    void SetDirection(const glm::vec3& direction);
    void SetPosition(const float position[3]);
    void SetPosition(const glm::vec3& position);
    void SetPositionX(const float& x);
    void SetPositionY(const float& y);
    void SetPositionZ(const float& z);
    void SetTarget(const float& x, const float& y, const float& z);
    void SetTarget(const float target[3]);
    void SetTarget(const glm::vec3& target);

    void ProcessKeyboardMovement(const float& deltaTime);
    void ProcessMouseMovement();
    void ProcessMouseScroll();

    void EnableControll();
    void DisableControll();
private:
    virtual void UpdateProjMatrix() = 0;
    void UpdateCameraFront(const glm::vec3& direction);
    void UpdateViewMatrix();
    void UpdatePitchAndYaw();
    float GetYawByDirection(const glm::vec3& direction);
    float GetPitchByDirection(const glm::vec3& direction);
private:
    CameraType m_Type;
protected:
    float m_ZoomLevel = 1.0f;
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