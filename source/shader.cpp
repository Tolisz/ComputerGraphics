#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>

shader::shader() 
{}

shader::~shader()
{
    glDeleteProgram(m_ID);
}

std::string shader::ReadShaderCode(const std::string& codeFilePath)
{
    std::string code;
    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        shaderFile.open(codeFilePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout 
            << "Shader code read error! \n\t[path]: " << codeFilePath
            << "\n\t[error mmessage]: " << e.what() 
            << std::endl;
    }

    return code;
}

void shader::Init()
{
    m_ID = glCreateProgram();
}

void shader::AttachShader(const std::string& path, GLenum type)
{
    GLuint sh = glCreateShader(type);
    std::string source = ReadShaderCode(path);
    const char* source_c = source.c_str();
    glShaderSource(sh, 1, &source_c, NULL);
    glCompileShader(sh);

    GLint success;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint log_length;
        glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> message(log_length);
        glGetShaderInfoLog(sh, log_length, nullptr, message.data());
        
        std::cout 
            << "AttachShader error! \n\t[path]: " << path 
            << "\n\t[type]: " << type 
            << "\n\t[error message]: \n" << message.data() << std::endl;
        return;
    }

    glAttachShader(m_ID, sh);
    glDeleteShader(sh);
}

void shader::Link()
{
    glLinkProgram(m_ID);

    GLint success; 
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint log_length;
        glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> message(log_length);
        glGetProgramInfoLog(m_ID, log_length, nullptr, message.data());

        std::cout << "LinkProgram error! \n\t[error message]: \n" << message.data() << std::endl;
        return;
    }
}

void shader::Use()
{
    glUseProgram(m_ID);
}

    // *=*=*=*=*=*=*=*=*=*=*=*
    //    UNIFORM SETTERS
    // *=*=*=*=*=*=*=*=*=*=*=*

void shader::set1b(const char* name, GLboolean value)
{
    glUniform1i(glGetUniformLocation(m_ID, name), (int)value); 
}

void shader::set1f(const char* name, GLfloat v0)
{
    glUniform1f(glGetUniformLocation(m_ID, name), v0);
}

void shader::set3f(const char* name, GLfloat v0, GLfloat v1, GLfloat v2)
{
    glUniform3f(glGetUniformLocation(m_ID, name), v0, v1, v2);
}

void shader::set3fv(const char* name, const glm::vec3& value)
{
    glUniform3fv(glGetUniformLocation(m_ID, name), 1, (float*)&value);
}

void shader::setM4fv(const char* name, GLboolean transpose, const glm::mat4& matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, transpose, (float*)&matrix);
}