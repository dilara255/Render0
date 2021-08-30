#include "input0.hpp"
#include "render0/camera.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void processKeyBindings(bool *input, int keyBound, int key, int action) {
	if (key == keyBound && action == GLFW_PRESS) {
		*input = true;
	}
	else if (key == keyBound && action == GLFW_RELEASE) {
		*input = false;
	}
}

namespace iz {

	int init(GLFWwindow* window) {

		RZ_TRACE("Vai iniciar input..");

		glfwSetKeyCallback(window, key_callback);
		RZ_INFO("Setou callback input para janela");

		return 1;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	struct keyBinding { bool* input; int keyBound; };
	const int keysToBind = 32;
	keyBinding bindings[keysToBind] = {

		& keyboardInput.leftPressed		, GLFW_KEY_A,
		& keyboardInput.rightPressed	, GLFW_KEY_D,
		& keyboardInput.upPressed	    , GLFW_KEY_W,
		& keyboardInput.downPressed	    , GLFW_KEY_S,
		& keyboardInput.forwardPressed	, GLFW_KEY_UP,
		& keyboardInput.backwardPressed	, GLFW_KEY_DOWN,

		& keyboardInput.pitchDownPressed    , GLFW_KEY_U,
		& keyboardInput.pitchUpPressed    , GLFW_KEY_I,
		& keyboardInput.yawLeftPressed      , GLFW_KEY_H,
		& keyboardInput.yawRightPressed      , GLFW_KEY_J,
		& keyboardInput.rollCWPressed     , GLFW_KEY_L,
		& keyboardInput.rollCCWPressed     , GLFW_KEY_K,

		& keyboardInput.increaseFOVPressed	, GLFW_KEY_PAGE_UP,
		& keyboardInput.decreaseFOVPressed	, GLFW_KEY_PAGE_DOWN,

		& keyboardInput.increaseNear	, GLFW_KEY_KP_MULTIPLY,
		& keyboardInput.decreaseNear	, GLFW_KEY_KP_DIVIDE,
		& keyboardInput.increaseFar	    , GLFW_KEY_KP_ADD,
		& keyboardInput.decreaseFar	    , GLFW_KEY_KP_SUBTRACT,

		& keyboardInput.toggleMoveCameraOrWorldCoords	, GLFW_KEY_C,
		& keyboardInput.toggleLookatPressed	            , GLFW_KEY_T,
		& keyboardInput.toggleOrthoPerspectivePressed	, GLFW_KEY_P,
		& keyboardInput.resetViewPressed	            , GLFW_KEY_R,

		& keyboardInput.increaseR	, GLFW_KEY_KP_7,
		& keyboardInput.increaseG	, GLFW_KEY_KP_8,
		& keyboardInput.increaseB	, GLFW_KEY_KP_9,
		& keyboardInput.decreaseR	, GLFW_KEY_KP_4,
		& keyboardInput.decreaseG	, GLFW_KEY_KP_5,
		& keyboardInput.decreaseB	, GLFW_KEY_KP_6,

		& keyboardInput.cycleDrawModePressed , GLFW_KEY_M,
		& keyboardInput.toggleCullingPressed , GLFW_KEY_X,
		& keyboardInput.toggleCCWtoCWPressed , GLFW_KEY_PERIOD,

		& keyboardInput.closeWindowPressed , GLFW_KEY_ESCAPE
	};

	for (int i = 0; i < keysToBind; i++) {
		processKeyBindings( bindings[i].input, bindings[i].keyBound
			              , key, action);
	}
};
