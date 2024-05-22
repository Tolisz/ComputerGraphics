#pragma once

#include <glm/mat4x4.hpp>

class camera
{

    glm::vec3 m_worldPos;
    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;

public:

    // *=*=*=*=*=*=*=*=*=*=
    //   Object Creation
    // *=*=*=*=*=*=*=*=*=*=

    camera();
    ~camera();

    camera(const camera&) = delete;
    camera(camera&&) = delete;
    camera& operator=(const camera&) = delete;
    camera& operator=(camera&&) = delete;

    // *=*=*=*=*=*=*=*=*=*=
    //      Methods
    // *=*=*=*=*=*=*=*=*=*=

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();

    
};