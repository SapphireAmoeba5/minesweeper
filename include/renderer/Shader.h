#pragma once

#include <glad/glad.h>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

public:
	void Bind() const;
	void Unbind() const;

	int GetUniformLocation(const std::string& name);

	void SetUniform1iv(const std::string& name, int count, int* i0);

	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
	void SetUniformMat2f(const std::string& name, const glm::mat2& matrix);

	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void SetUniform4f(const std::string& name, float* floats);
	void SetUniform4f(const std::string& name, const glm::vec4& vec);

	void SetUniform3f(const std::string& name, float f0, float f1, float f2);
	void SetUniform3f(const std::string& name, float* floats);
	void SetUniform3f(const std::string& name, const glm::vec3& vec);

	void SetUniform2f(const std::string& name, float f0, float f1);
	void SetUniform2f(const std::string& name, float* floats);
	void SetUniform2f(const std::string& name, const glm::vec2& vec);

	void SetUniform1f(const std::string& name, float f0);

	void SetUniform1i(const std::string& name, int i0);

public:
	inline const size_t GetObjectCount() const { return m_ObjectCount; }

private:
	uint32_t m_RendererID;

private:
	void Impl_CheckCompileErrors(unsigned int shader, const std::string& type);
private:
	static size_t m_ObjectCount;
};

