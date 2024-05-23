#include "camera.h"

#include <glm/ext/matrix_transform.hpp>     // glm::lookAtRH
#include <glm/ext/matrix_clip_space.hpp>    // glm::perspective

#include <iostream>

camera::camera()
    : m_worldPos(0.0f, 0.0f, -3.0f),
      m_front(0.0f, 0.0f, 1.0f),
      m_right(1.0f, 0.0f, 0.0f),
      m_up(0.0f, 1.0f, 0.0f),
      m_viewCenter(0.0f, 0.0f, 0.0f),

      m_nearPlane(0.01f),
      m_farPlane(1000.0f),
      m_fov(45.0f)
{}

camera::~camera()
{
    
}

glm::mat4 camera::GetViewMatrix()
{
    return glm::lookAtRH(m_worldPos, m_worldPos + m_front, m_up);
}

glm::mat4 camera::GetProjectionMatrix(float aspect)
{
    return glm::perspectiveRH(glm::radians(m_fov), aspect, m_nearPlane, m_farPlane);
}

void camera::UpdatePosition(float posDelta)
{
    std::cout << m_worldPos.x << ", " << m_worldPos.x << ", " << m_worldPos.z  << std::endl;
    m_worldPos += m_front * posDelta;
}

void camera::UpdateRotation(glm::vec3 rotDelta)
{
    /* TODO później */
}

