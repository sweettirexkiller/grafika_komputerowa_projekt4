#pragma once
#include <string>
#include <unordered_map>

class Shader
{
	private:
		unsigned int m_RendererID;
		std::string m_FilePath;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:	
		Shader(const std::string& filepath);
		~Shader();
		void Bind() const;
		void Unbind() const;

		// Set uniforms
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniform1i(const std::string& name, int value);
		void SetUniformMat4f(const std::string& name, const float* matrix);
	private:
		int GetUniformLocation(const std::string& name);
};