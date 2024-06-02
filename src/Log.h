#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

class Log
{
private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;

public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }
};

// Core log macros
#define CORE_TRACE(...) ::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)  ::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)  ::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...) ::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define TRACE(...) ::Log::GetClientLogger()->trace(__VA_ARGS__)
#define INFO(...)  ::Log::GetClientLogger()->info(__VA_ARGS__)
#define WARN(...)  ::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ERROR(...) ::Log::GetClientLogger()->error(__VA_ARGS__)
#define FATAL(...) ::Log::GetClientLogger()->fatal(__VA_ARGS__)

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

#define ASSERT(x) if(!(x)) triggerBreakpoint();
#define CORE_ASSERT(x, ...) { if(!(x)) { CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK; } }
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void triggerBreakpoint();
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

