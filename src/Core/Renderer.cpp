#include "Renderer.h"

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

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const DrawType& drawType) const
{
    unsigned int glType;
    switch (drawType)
    {
    case DrawType::Traingles:
        glType = GL_TRIANGLES;
        break;
    case DrawType::Points:
        glType = GL_POINTS;
        break;
    case DrawType::Lines:
        glType = GL_LINES;
        break;
    }

    va.Bind();
    GLCall(glDrawArrays(glType, 0, va.GetCount()));
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer& ib, const DrawType& drawType) const
{
    unsigned int glType;
    switch (drawType)
    {
    case DrawType::Traingles:
        glType = GL_TRIANGLES;
        break;
    case DrawType::Points:
        glType = GL_POINTS;
        break;
    case DrawType::Lines:
        glType = GL_LINES;
        break;
    }

    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(glType, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const Shader &shader, const VertexArray &va) const
{
    shader.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, va.GetCount()));
}

void Renderer::Draw(const Shader &shader, const VertexArray &va, const IndexBuffer& ib) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
