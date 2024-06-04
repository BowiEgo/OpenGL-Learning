#include "Camera.h"

#include "Log.h"
#include "Input.h"
#include "KeyCodes.h"

Camera::Camera(float fov, float aspectRatio, const float position[3], const float target[3])
    : m_FOV(fov), m_AspectRatio(aspectRatio)
{
    if (position)
    {
        m_CameraPos = glm::vec3(position[0], position[1], position[2]);
    }
    else
    {
        m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    }

    if (target)
    {
        m_CameraTarget = glm::vec3(target[0], target[1], target[2]);
    }
    else
    {
        m_CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    m_CameraFront = glm::normalize(m_CameraTarget - m_CameraPos);
    m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_CameraUp));
}

Camera::~Camera()
{
}

void Camera::SetPosition(std::initializer_list<float> position)
{
    if (position.size() == 3) {
        auto it = position.begin();
        m_CameraPos = glm::vec3(*it, *(it + 1), *(it + 2));
    }
}

void Camera::ProcessKeyboardMovement(float deltaTime)
{
    float cameraSpeed = 2.5f * deltaTime;
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
    Input::ProcessMove();

    float sensitivity = 0.05f;

    m_Yaw += Input::GetMouseOffsetX() * sensitivity;
    m_Pitch -= Input::GetMouseOffsetY() * sensitivity;

    if(m_Pitch > 89.0f)
        m_Pitch =  89.0f;
    if(m_Pitch < -89.0f)
        m_Pitch = -89.0f;

    glm::vec3 front(0.0f, 0.0f, -1.0f);
    front.x = sin(glm::radians(m_Yaw));
    front.y = sin(glm::radians(m_Pitch));
    m_CameraFront = glm::normalize(front);
}

void Camera::ProcessMouseScroll()
{
    float yScrollOffset = Input::GetMouseScroll();

    if(m_FOV >= 1.0f && m_FOV <= 45.0f)
        m_FOV -= yScrollOffset * 1.0;
    if(m_FOV <= 1.0f)
        m_FOV = 1.0f;
    if(m_FOV >= 45.0f)
        m_FOV = 45.0f;
}
