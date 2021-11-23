
#include "renderer/Shader.h"

#include <GLFW/glfw3.h>

#include <sstream>
#include <fstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    glfwGetCurrentContext();

    m_ObjectCount++;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);

	std::stringstream vShaderStream, fShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	
	vShaderFile.close();
	fShaderFile.close();

    std::string vertexCodeStr = vShaderStream.str();
    std::string fragmentCodeStr = fShaderStream.str();

    const char* vertexCode = vertexCodeStr.c_str();
    const char* fragmentCode = fragmentCodeStr.c_str();

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
    Impl_CheckCompileErrors(vertex, "VERTEX");
	
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);
    Impl_CheckCompileErrors(fragment, "FRAGMENT");

    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertex);
    glAttachShader(m_RendererID, fragment);
    glLinkProgram(m_RendererID);
    Impl_CheckCompileErrors(m_RendererID, "PROGRAM");

    glUseProgram(m_RendererID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glfwGetCurrentContext();
    m_ObjectCount--;
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::Impl_CheckCompileErrors(unsigned int shader, const std::string& type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}


int Shader::GetUniformLocation(const std::string& name)
{
    int loc = glGetUniformLocation(m_RendererID, name.c_str());

    return loc;
}

void Shader::SetUniform1iv(const std::string& name, int count, int* i0)
{
    int loc = GetUniformLocation(name);
    glUniform1iv(loc, count, i0);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    int loc = GetUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
    int loc = GetUniformLocation(name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniformMat2f(const std::string& name, const glm::mat2& matrix)
{
    int loc = GetUniformLocation(name);
    glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    glUniform4f(GetUniformLocation(name), f0, f1, f2, f3);
}

void Shader::SetUniform4f(const std::string& name, float* floats)
{
    glUniform4f(GetUniformLocation(name), floats[0], floats[1], floats[2], floats[3]);
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& vec)
{
    glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniform3f(const std::string& name, float f0, float f1, float f2)
{
    glUniform3f(GetUniformLocation(name), f0, f1, f2);
}

void Shader::SetUniform3f(const std::string& name, float* floats)
{
    glUniform3f(GetUniformLocation(name), floats[0], floats[1], floats[2]);
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& vec)
{
    glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
}

void Shader::SetUniform2f(const std::string& name, float f0, float f1)
{
    glUniform2f(GetUniformLocation(name), f0, f1);
}

void Shader::SetUniform2f(const std::string& name, float* floats)
{
    glUniform2f(GetUniformLocation(name), floats[0], floats[1]);
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& vec)
{
    glUniform2f(GetUniformLocation(name), vec.x, vec.y);
}

void Shader::SetUniform1f(const std::string& name, float f0)
{
    glUniform1f(GetUniformLocation(name), f0);
}

void Shader::SetUniform1i(const std::string& name, int i0)
{
    glUniform1i(GetUniformLocation(name), i0);
}


size_t Shader::m_ObjectCount = 0;