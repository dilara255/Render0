#pragma once

#include <memory>

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace util {

	class Log
	{
	public:
		RZ_API static void init();
		inline static std::shared_ptr<spdlog::logger>& GetRenderLogger() {
			if (!initialized) init();  return s_RenderLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetViewerLogger() {
			if (!initialized) init(); return s_ViewerLogger;
		}

	private:
		RZ_API static std::shared_ptr<spdlog::logger> s_RenderLogger;
		RZ_API static std::shared_ptr<spdlog::logger> s_ViewerLogger;
		RZ_API static int initialized;
	};

}