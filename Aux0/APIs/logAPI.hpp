#pragma once

#include "core.hpp"

#ifndef RZ_BUILD_LIB
    #include "../include/log.hpp"
#endif

namespace util {
    void rzLog(const char* message, std::shared_ptr<spdlog::logger> logger
        , const int degree, const char* file, const int line);
}

#define L_TRACE 0
#define L_INFO 1
#define L_WARN 2
#define L_ERROR 3
#define L_CRITICAL 4

//Eventualmente criar macros "DEF_IFNOT_DISTRIB"?

//MACROS log Render0
#ifdef DISTRIB
    #define RZ_CRITICAL(...)
#else
    #define RZ_CRITICAL(...) util::rzLog(__VA_ARGS__, util::Log::GetRenderLogger()\
                                        , L_CRITICAL, __FILE__, __LINE__)
#endif

#ifdef DISTRIB
    #define RZ_ERROR(...)
#else
    #define RZ_ERROR(...)    util::rzLog(__VA_ARGS__, util::Log::GetRenderLogger()\
                                        , L_ERROR, __FILE__, __LINE__)
#endif

#ifdef DISTRIB
    #define RZ_WARN(...)
#else
    #define RZ_WARN(...)     util::rzLog(__VA_ARGS__, util::Log::GetRenderLogger()\
                                         , L_WARN, __FILE__, __LINE__)
#endif

#ifdef DISTRIB
    #define RZ_INFO(...)
#else
    #define RZ_INFO(...)     util::rzLog(__VA_ARGS__, util::Log::GetRenderLogger()\
                                         , L_INFO, __FILE__, __LINE__)
#endif

#ifdef DISTRIB
    #define RZ_TRACE(...)
#else
    #define RZ_TRACE(...)    util::rzLog(__VA_ARGS__, util::Log::GetRenderLogger()\
                                        , L_TRACE, __FILE__, __LINE__)
#endif

//MACROS log Viewr0
#ifdef DISTRIB
    #define VZ_CRITICAL(...)
#else
    #define VZ_CRITICAL(...) util::rzLog(__VA_ARGS__, util::Log::GetViewerLogger()\
                                        , L_CRITICAL, __FILE__, __LINE__)
#endif

#ifdef DISTRIB
    #define VZ_ERROR(...)
#else
    #define VZ_ERROR(...)    util::rzLog(__VA_ARGS__, util::Log::GetViewerLogger()\
                                         , L_ERROR, __FILE__, __LINE__)
#endif

#ifdef DISTRIB
    #define VZ_WARN(...)
#else
    #define VZ_WARN(...)     util::rzLog(__VA_ARGS__, util::Log::GetViewerLogger()\
                                        , L_WARN, __FILE__, __LINE__)
#endif

#ifdef DISTRIB
    #define VZ_INFO(...)
#else
    #define VZ_INFO(...)     util::rzLog(__VA_ARGS__, util::Log::GetViewerLogger()\
                                        , L_INFO, __FILE__, __LINE__)
#endif

#ifdef DISTRIB
    #define VZ_TRACE(...)
#else
    #define VZ_TRACE(...)    util::rzLog(__VA_ARGS__, util::Log::GetViewerLogger()\
                                         , L_TRACE, __FILE__, __LINE__)
#endif