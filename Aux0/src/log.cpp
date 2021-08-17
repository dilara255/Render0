#include "miscStdHeaders.h"
#include "log.hpp"
#include "logAPI.hpp"

namespace util {

	std::shared_ptr<spdlog::logger> Log::s_RenderLogger;
	std::shared_ptr<spdlog::logger> Log::s_ViewerLogger;
	int Log::initialized = 0;

	void Log::init() {

		if (!Log::initialized) {

			spdlog::set_pattern("%^[%T] %n: %v%$");
			s_RenderLogger = spdlog::stdout_color_mt("RENDER");
			s_RenderLogger->set_level(spdlog::level::trace);

			s_ViewerLogger = spdlog::stdout_color_mt("VIEWER");
			s_ViewerLogger->set_level(spdlog::level::trace);

			Log::initialized = true;
		}

		if (initialized) {
			RZ_INFO("Render Logger initialized");
			VZ_INFO("Viewer Logger initialized");
		}
		else std::cerr << "\n\nFALHA AO INICIALIZAR LOGGERS\n\n";
	}


	void rzLog(const char* message, std::shared_ptr<spdlog::logger> logger
		       , const int degree, const char* file, const int line) {
#ifdef DEBUG
		std::cout << "\t-> " << file << " @ line " << line << ":" << std::endl;
#endif
		switch (degree) {
		case L_TRACE:
			logger->trace(message);
			break;
		case L_INFO:
			logger->info(message);
			break;
		case L_WARN:
			logger->warn(message);
			break;
		case L_ERROR:
			logger->error(message);
			break;
		case L_CRITICAL:
			logger->critical(message);
			break;
		default:
		{
			logger->critical("Error on logging degree: will show as critical:");
			logger->critical(message);
		}
		}
	}
}

