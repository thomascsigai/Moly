#pragma once

#include <memory>
#include <Core.h>
#include <spdlog/spdlog.h>

namespace Moly 
{
    class MOLY_API Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define ML_CORE_FATAL(...)      ::Moly::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define ML_CORE_ERROR(...)      ::Moly::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ML_CORE_WARN(...)       ::Moly::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ML_CORE_INFO(...)       ::Moly::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ML_CORE_TRACE(...)      ::Moly::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client log macros
#define ML_CLIENT_FATAL(...)    ::Moly::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define ML_CLIENT_ERROR(...)    ::Moly::Log::GetClientLogger()->error(__VA_ARGS__)
#define ML_CLIENT_WARN(...)     ::Moly::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ML_CLIENT_INFO(...)     ::Moly::Log::GetClientLogger()->info(__VA_ARGS__)
#define ML_CLIENT_TRACE(...)    ::Moly::Log::GetClientLogger()->trace(__VA_ARGS__)