#ifndef SHADER_H

#define SHADER_H

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

class Shader
{
	public:
		unsigned int ID;
		Shader(const char* vertexPath, const char* fragmentPath);
		Shader(const int programID);
		~Shader();

		void use();
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;

};



























#endif // !SHADER_H
