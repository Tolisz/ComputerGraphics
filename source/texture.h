#pragma once

#include <glad/glad.h>

class texture
{
private:

    GLuint m_ID;

public:

    // *=*=*=*=*=*=*=*=*=*=
    //   Object Creation
    // *=*=*=*=*=*=*=*=*=*=

    texture();
    ~texture();

    texture(const texture&) = delete;
    texture(texture&&);
    texture& operator=(const texture&) = delete;
    texture& operator=(texture&&);

    static texture CreateTexture2D(
        GLint wrapS, 
        GLint wrapT,
        GLint filterMin,
        GLint filterMag);
    
};