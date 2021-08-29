#pragma once

#include <GLFW/glfw3.h>
#include "logAPI.hpp"

namespace iz {
	int init(GLFWwindow* window);

	typedef struct inputGiven_st {
		bool leftPressed = false;
		bool rightPressed = false;
		bool upPressed = false;
		bool downPressed = false;
		bool forwardPressed = false;
		bool backwardPressed = false;
		bool toggleOrthoPerspectivePressed = false;
		bool toggleLookatPressed = false;
		bool toggleMoveCameraOrWorldCoords = false;
		bool resetViewPressed = false;
		bool increaseFOVPressed = false;
		bool decreaseFOVPressed = false;
		bool increaseNear = false;
		bool decreaseNear = false;
		bool increaseFar = false;
		bool decreaseFar = false;
		bool increaseR = false;
		bool increaseG = false;
		bool increaseB = false;
		bool decreaseR = false;
		bool decreaseG = false;
		bool decreaseB = false;
	} inputGiven_t;
}

inline iz::inputGiven_t keyboardInput;