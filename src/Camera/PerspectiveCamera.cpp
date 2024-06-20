#include "PerspectiveCamera.h"

#include "Input.h"
#include "KeyCodes.h"

PerspectiveCamera::PerspectiveCamera(const float position[3], const float target[3], float fov, float aspectRatio, float near, float far)
    : m_FOV(fov), m_AspectRatio(aspectRatio), m_Near(near), m_Far(far), m_Type(CameraType::Perspective)
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

    Init();
    Input::DisableCursor();
}

PerspectiveCamera::~PerspectiveCamera()
{
    Input::EnableCursor();
}

void PerspectiveCamera::SetFOV(const float &fov)
{
    m_FOV = fov;
    UpdateProjMatrix();
}

void PerspectiveCamera::SetAspectRatio(const float& aspectRatio)
{
    if (aspectRatio == m_AspectRatio)
        return;

    m_AspectRatio = aspectRatio;
    UpdateProjMatrix();
}

void PerspectiveCamera::UpdateProjMatrix()
{
    m_ProjMatrix = glm::perspective(glm::radians(m_FOV * m_ZoomLevel), m_AspectRatio, m_Near, m_Far);
}
