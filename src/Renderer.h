#pragma once

#include <GL/glew.h>

//c++ macro, compiler intrinsic function
#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);