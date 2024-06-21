#include "OrthographicCamera.h"

#include "Input.h"
#include "KeyCodes.h"

OrthographicCamera::OrthographicCamera(const float position[3], const float target[3], float near, float far)
    : m_Near(near), m_Far(far), m_Type(CameraType::Orthographic)
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

OrthographicCamera::~OrthographicCamera()
{
    Input::EnableCursor();
}

void OrthographicCamera::UpdateProjMatrix()
{
    m_ProjMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 7.5f);
}
