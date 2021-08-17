#include "RZ_api.hpp"
#include "logAPI.hpp"

void mainLoop1();
void mainLoop2();

int main(void){

	VZ_TRACE("\nIniciando Render...");

	if (!renderz::init()) VZ_CRITICAL("\nRenderizador nao iniciou...");

	/* Loop until the user closes the window */
	mainLoop1();
	
	/* Terminate GLFW*/
	renderz::terminate();

	VZ_TRACE("Readquirir contexto...");

	if (!renderz::init()) VZ_CRITICAL("\nContexto nao voltou!");

	VZ_INFO("Contexto Readquirido...");

	/* Loop until the user closes the window */
	mainLoop2();

	/* Terminate GLFW*/
	renderz::terminate();

	VZ_TRACE("\nEnter para fechar...");
	getc(stdin);
	
	return 1;
}

void mainLoop1() {
	bool keepGoing = true;
	while (keepGoing) {
		keepGoing = renderz::render();
	}
	VZ_INFO("\nJanela fechada...");
}

void mainLoop2() {
	renderz::triangles();
	VZ_INFO("\nJanela fechada...");
}