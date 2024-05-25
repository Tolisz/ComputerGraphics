#pragma once 

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

class light
{
public:
    // *=*=*=*=*=*=*=*=*=*=
    //   Light parameters
    // *=*=*=*=*=*=*=*=*=*=

    glm::vec3 m_position;
    glm::vec3 m_diffuseColor;
    glm::vec3 m_specularColor;

private:
    // *=*=*=*=*=*=*=*=*=*=
    //    OpenGL things
    // *=*=*=*=*=*=*=*=*=*=

    static bool m_gl_isInit;

    static GLuint m_gl_VAO;
    static GLuint m_gl_VBO;
    static GLuint m_gl_EBO;

    static const glm::vec3 m_gl_vertices[4];
    static const glm::uvec3 m_gl_indices[2];

public:
    // *=*=*=*=*=*=*=*=*=*=
    //   Object Creation
    // *=*=*=*=*=*=*=*=*=*=

    light();
    ~light();

    light(const light&) = default;
    light(light&&) = default;
    light& operator=(const light&) = delete;
    light& operator=(light&&) = delete;

    // *=*=*=*=*=*=*=*=*=*=
    //       InitGL
    // *=*=*=*=*=*=*=*=*=*=

    void InitGL();
    void DeInitGL();
    
    void Draw();

private:

};
