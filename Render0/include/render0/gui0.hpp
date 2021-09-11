#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "render0.hpp"
#include "logAPI.hpp"

#include "RZ_api.hpp"

enum guiWindows { GUI_MAIN, GUI_STATS, NUMBER_GUI_WINDOWS };
inline bool guiWindowsToShow[NUMBER_GUI_WINDOWS];

namespace gz {
	void init(GLFWwindow* window, renderInfo_t* renderInfo_ptr, CameraZ* camera_ptr);

	void startGuiFrame();
	void showGuiWindows();
	void renderGui();
    void updateStateFromGui();

	void terminate();

	inline bool imGuiOn = false;

	struct toggleOption_st {
		bool currentState;
		bool lastState;
	};

	struct toggle_st {
        const char* description;
		toggleOption_st* optionsArray;
        const char** optionDescriptionsArray;
		int numberOptions;
	};

    struct guiControlVariables_st {
        renderInfo_t* renderInfo_ptr;
        CameraZ* camera_ptr;

        float fovV;
        float fovH;
        float nearDist;
        float farDist;

        toggleOption_st useLookAt;
        toggleOption_st backfaceCull;
        toggleOption_st useVsync;

        int swapInterval;
        int orientation;

        toggleOption_st projectionArr[2];
        const char* projDescriptions[2] = { "Perspective", "Orthogonal" };
        toggle_st projection = { "Projection Type : ", & projectionArr[0], 
                                 &projDescriptions[0], 2};

        toggleOption_st rendererArr[2];
        const char* rendererDescriptions[2] = { "OGL", "C2GL" };
        toggle_st renderer = { "Rendering Pipeline : ", &rendererArr[0],
                                 & rendererDescriptions[0], 2 };

        toggleOption_st faceOrientationArr[2];
        const char* faceOrientationDescriptions[2] = { "CW", "CCW" };
        toggle_st faceOrientation = { "Face Orientation : ", &faceOrientationArr[0],
                                 & faceOrientationDescriptions[0], 2 };

        toggleOption_st shaderArr[5];
        const char* shaderDescriptions[5] = { "None", "Flat", "Gouroud", 
                                            "Phong/Vertex", "Phong/Frag" };
        toggle_st shader = { "Shading: ", &shaderArr[0],
                                 &shaderDescriptions[0], 5 };
    };
}