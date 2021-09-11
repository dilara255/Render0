#pragma once

//define struct que receberá estado de controles
//input.cpp faz a ligação entre inputs de fato e essas variáveis de estado

#include <GLFW/glfw3.h>
#include "logAPI.hpp"

#include "render0/gui0.hpp"

namespace iz {
	int init(GLFWwindow* window);
	void processInput();

	typedef struct inputGiven_st {
		bool leftPressed     = false;
		bool rightPressed    = false;
		bool upPressed       = false;
		bool downPressed     = false;
		bool forwardPressed  = false;
		bool backwardPressed = false;

		bool toggleOrthoPerspectivePressed = false;
		bool toggleLookatPressed           = false;
		bool toggleMoveCameraOrWorldCoords = false;

		bool resetViewPressed = false;

		bool increaseFOVPressed = false;
		bool decreaseFOVPressed = false;
		bool increaseNear       = false;
		bool decreaseNear       = false;
		bool increaseFar        = false;
		bool decreaseFar        = false;

		bool increaseR = false;
		bool increaseG = false;
		bool increaseB = false;
		bool decreaseR = false;
		bool decreaseG = false;
		bool decreaseB = false;

		bool cycleDrawModePressed = false;
		bool toggleCullingPressed = false;
		bool toggleCCWtoCWPressed = false;
		bool toggleOglCtgl        = false;
		bool toggleVsync          = false;

		bool closeWindowPressed = false;

		bool pitchDownPressed = false;
		bool pitchUpPressed   = false;
		bool yawLeftPressed   = false;
		bool yawRightPressed  = false;
		bool rollCWPressed    = false;
		bool rollCCWPressed   = false;
	} inputGiven_t;
}

inline iz::inputGiven_t keyboardInput; //encapsular melhor