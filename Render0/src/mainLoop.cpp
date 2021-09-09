#include "render0/render0.hpp"

void mainLoop(renderInfo_t*  renderInfo_ptr, CameraZ* camera_ptr,
	          float* modelMatrixStart_ptr,
	          void (*controlTest) (CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr)) {

	bool keepRunning = true;

	RZ_INFO("Iniciando loop principal");

	while (keepRunning) {
		keepRunning = render(renderInfo_ptr, camera_ptr, modelMatrixStart_ptr, controlTest);
		glfwPollEvents();
		controlTest(camera_ptr, renderInfo_ptr);
	}

	RZ_TRACE("Loop principal encerrado!");
}