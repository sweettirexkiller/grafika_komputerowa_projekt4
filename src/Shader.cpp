#include "Shader.h"

Shader::Shader(const std::string& filepath)
{
}

Shader::~Shader()
{
}
void Shader::Bind() const
{
}
void Shader::Unbind() const
{
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
}
void Shader::SetUniform1i(const std::string& name, int value)
{
}
void Shader::SetUniformMat4f(const std::string& name, const float* matrix)
{
}
int Shader::GetUniformLocation(const std::string& name)
{
}