#include "Camera/Camera.h"

#include "Input.h"
#include "KeyCodes.h"
#include "Camera.h"

Camera::~Camera()
{
    Input::EnableCursor();
}

void Camera::Init()
{
    m_CameraFront = glm::normalize(m_CameraTarget - m_CameraPos);
    m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_CameraUp));
    UpdateProjMatrix();
    m_ViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);

    m_Yaw = GetYawByDirection(m_CameraFront);
    m_Pitch = GetPitchByDirection(m_CameraFront);

    UpdatePitchAndYaw();
}

void Camera::SetZoomLevel(const float &zoomlv)
{
    m_ZoomLevel = zoomlv;
    UpdateProjMatrix();
}

void Camera::SetNear(const float& near)
{
    m_Near = near;
}

void Camera::SetFar(const float& far)
{
    m_Far = far;
}

void Camera::SetDirection(const glm::vec3 &direction)
{
    m_CameraFront = direction;
    Init();
}

void Camera::SetPosition(const float position[3])
{
    if (position[0] == m_CameraPos.x && position[1] == m_CameraPos.y && position[2] == m_CameraPos.z)
        return;

    m_CameraPos = glm::vec3(position[0], position[1], position[2]);
    m_CameraFront = glm::normalize(m_CameraTarget - m_CameraPos);
    m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_CameraUp));

    m_Yaw = GetYawByDirection(m_CameraFront);
    m_Pitch = GetPitchByDirection(m_CameraFront);

    UpdatePitchAndYaw();
}

void Camera::SetPosition(const glm::vec3& position)
{
    if (position == m_CameraPos)
        return;

    m_CameraPos = position;
    m_CameraFront = glm::normalize(m_CameraTarget - m_CameraPos);
    m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_CameraUp));

    m_Yaw = GetYawByDirection(m_CameraFront);
    m_Pitch = GetPitchByDirection(m_CameraFront);

    UpdatePitchAndYaw();
}

void Camera::SetPositionX(const float& x)
{
    if (x == m_CameraPos.x)
        return;

    m_CameraPos.x = x;
    m_CameraFront = glm::normalize(m_CameraTarget - m_CameraPos);
    m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_CameraUp));

    m_Yaw = GetYawByDirection(m_CameraFront);
    m_Pitch = GetPitchByDirection(m_CameraFront);

    UpdatePitchAndYaw();
}

void Camera::SetPositionY(const float& y)
{
    if (y == m_CameraPos.y)
        return;

    m_CameraPos.y = y;
    Init();
}

void Camera::SetPositionZ(const float& z)
{
    if (z == m_CameraPos.z)
        return;

    m_CameraPos.z = z;
    m_CameraFront = glm::normalize(m_CameraTarget - m_CameraPos);
    m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_CameraUp));

    m_Yaw = GetYawByDirection(m_CameraFront);
    m_Pitch = GetPitchByDirection(m_CameraFront);

    UpdatePitchAndYaw();
}

void Camera::SetTarget(const float& x, const float& y, const float& z)
{
    glm::vec3 target = glm::vec3(x, y, z);
    SetTarget(target);
}

void Camera::SetTarget(const float target[3])
{
    glm::vec3 targetVec = glm::vec3(target[0], target[1], target[2]);
    SetTarget(targetVec);
}

void Camera::SetTarget(const glm::vec3& target)
{
    CORE_TRACE("{0}, {1}, {2}", target.x, target.y, target.z);

    if (target == m_CameraTarget)
        return;
    m_CameraTarget = glm::vec3(target[0], target[1], target[2]);
    Init();
}

void Camera::ProcessKeyboardMovement(const float& deltaTime)
{
    if (!m_Controll_Enabled)
        return;

    float cameraSpeed = m_MoveSpeed * deltaTime;
    if (Input::IsKeyPressed(OPL_KEY_W))
        m_CameraPos += cameraSpeed * m_CameraFront;
    if (Input::IsKeyPressed(OPL_KEY_S))
        m_CameraPos -= cameraSpeed * m_CameraFront;
    if (Input::IsKeyPressed(OPL_KEY_A))
        m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
    if (Input::IsKeyPressed(OPL_KEY_D))
        m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
}

void Camera::ProcessMouseMovement()
{
    if (m_FirstMouse)
    {
        m_LastX = Input::GetMouseX();
        m_LastY = Input::GetMouseY();
        m_FirstMouse = false;
    }

    float xOffset = Input::GetMouseX() - m_LastX;
    float yOffset = m_LastY - Input::GetMouseY(); 
    m_LastX = Input::GetMouseX();
    m_LastY = Input::GetMouseY();

    if (!m_Controll_Enabled)
        return;

    float sensitivity = 1.0f;

    m_Yaw += xOffset * m_YawSpeed * sensitivity;
    m_Pitch += yOffset * m_PitchSpeed * sensitivity;

    if(m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    if(m_Pitch < -89.0f)
        m_Pitch = -89.0f;
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    direction.y = sin(glm::radians(m_Pitch));
    direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    UpdateCameraFront(direction);
}

void Camera::ProcessMouseScroll()
{
    if (!m_Controll_Enabled)
        return;

    float yScrollOffset = Input::GetMouseScroll();

    if (yScrollOffset != m_LastScroll)
    {
        if(m_ZoomLevel >= 0.1f && m_ZoomLevel <= 3.14f)
            m_ZoomLevel -= yScrollOffset * 0.05;
        if(m_ZoomLevel <= 0.1f)
            m_ZoomLevel = 0.1f;
        if(m_ZoomLevel >= 3.14f)
            m_ZoomLevel = 3.14f;
        m_LastScroll = yScrollOffset;
        UpdateProjMatrix();
    }
}

void Camera::EnableControll()
{
    m_Controll_Enabled = true;
}

void Camera::DisableControll()
{
    m_Controll_Enabled = false;
}

void Camera::UpdateCameraFront(const glm::vec3 &direction)
{
    m_CameraFront = glm::normalize(direction);
    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
}

void Camera::UpdatePitchAndYaw()
{
    m_Yaw = GetYawByDirection(m_CameraFront);
    m_Pitch = GetPitchByDirection(m_CameraFront);
}

float Camera::GetYawByDirection(const glm::vec3& direction)
{
    return glm::degrees(atan2(direction.z, direction.x));
}

float Camera::GetPitchByDirection(const glm::vec3& direction)
{
    return glm::degrees(asin(direction.y));
}
