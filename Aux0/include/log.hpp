#pragma once

//Declaração funções log. Usa Spdlog. Exposto via macros em logAPI.hpp

#include "miscStdHeaders.h"

#include "core.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace util {

	class Log
	{
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& GetRenderLogger() {
			if (!initialized) init();  return s_RenderLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetViewerLogger() {
			if (!initialized) init(); return s_ViewerLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_RenderLogger;
		static std::shared_ptr<spdlog::logger> s_ViewerLogger;
		static int initialized;
	};

}