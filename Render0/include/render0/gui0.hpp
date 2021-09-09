#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "logAPI.hpp"

#include "RZ_api.hpp"

enum guiWindows { DEMO, DEMO2, NUMBER_GUI_WINDOWS };
inline bool guiWindowsToShow[NUMBER_GUI_WINDOWS];

namespace gz {
	void init(GLFWwindow* window);

	void startGuiFrame();
	void showGuiWindows(float* clearColor);
	void renderGui();

	void terminate();

	inline bool imGuiOn = false;
}