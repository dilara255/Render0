#pragma once

#include "core.h"
#ifndef RZ_BUILD_DLL
    #include "../util/Log.h"
#endif

namespace util {
    RZ_API void rzLog(const char* message, std::shared_ptr<spdlog::logger> logger
		             , const int degree, const char* file, const int line);
}

#define L_TRACE 0
#define L_INFO 1
#define L_WARN 2
#define L_ERROR 3
#define L_CRITICAL 4

//MACROS log Render0
#define RZ_CRITICAL(...) util::rzLog(__VA_ARGS__, util::Log::GetRenderLogger()\
                                    , L_CRITICAL, __FILE__, __LINE__)
#define RZ_ERROR(...)    util::rzLog(__VA_ARGS__, util::Log::GetRenderLogger()\
                                    , L_ERROR, __FILE__, __LINE__)
#define RZ_WARN(...)     util::rzLog(__VA_ARGS__, util::Log::GetRenderLogger()\
                                    , L_WARN, __FILE__, __LINE__)
#define RZ_INFO(...)     util::rzLog(__VA_ARGS__, util::Log::GetRenderLogger()\
                                    , L_INFO, __FILE__, __LINE__)
#define RZ_TRACE(...)    util::rzLog(__VA_ARGS__, util::Log::GetRenderLogger()\
                                    , L_TRACE, __FILE__, __LINE__)

//MACROS log Viewr0
#define VZ_CRITICAL(...) util::rzLog(__VA_ARGS__, util::Log::GetViewerLogger()\
                                    , L_CRITICAL, __FILE__, __LINE__)
#define VZ_ERROR(...)    util::rzLog(__VA_ARGS__, util::Log::GetViewerLogger()\
                                    , L_ERROR, __FILE__, __LINE__)
#define VZ_WARN(...)     util::rzLog(__VA_ARGS__, util::Log::GetViewerLogger()\
                                    , L_WARN, __FILE__, __LINE__)
#define VZ_INFO(...)     util::rzLog(__VA_ARGS__, util::Log::GetViewerLogger()\
                                    , L_INFO, __FILE__, __LINE__)
#define VZ_TRACE(...)    util::rzLog(__VA_ARGS__, util::Log::GetViewerLogger()\
                                    , L_TRACE, __FILE__, __LINE__)