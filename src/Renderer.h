#pragma once

#include <GL/glew.h>

#define ASSERT(x) if(!(x)) triggerBreakpoint();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void triggerBreakpoint();
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
