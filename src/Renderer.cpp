#include "Renderer.h"

#include <iostream>

#if defined(_MSC_VER)
    #define DEBUG_BREAK __debugbreak()
#elif defined(__APPLE__) || defined(__MACH__)
    #include <signal.h>
    #define DEBUG_BREAK __builtin_trap()
#elif defined(__linux__)
    #include <signal.h>
    #define DEBUG_BREAK raise(SIGTRAP)
#else
    #error "Platform not supported"
#endif

void triggerBreakpoint() {
    DEBUG_BREAK;
}

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error  = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "):" << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}