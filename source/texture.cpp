#include "texture.h"

#include <iostream>

texture::texture()
{
}

texture::~texture()
{
}

texture::texture(texture&& old)
{
    m_ID = old.m_ID;
    old.m_ID = 0;
}

texture& texture::operator=(texture&& old)
{
    m_ID = old.m_ID;
    old.m_ID = 0;
    return *this;
}

/* static */ texture texture::CreateTexture2D(
    GLint wrapS, 
    GLint wrapT,
    GLint filterMin,
    GLint filterMag)
{
    texture t;
    
    glGenTextures(1, &t.m_ID);
    glBindTexture(GL_TEXTURE_2D, t.m_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);
    glBindTexture(GL_TEXTURE_2D, 0);

    return std::move(t);
}

