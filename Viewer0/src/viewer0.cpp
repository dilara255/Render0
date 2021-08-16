#include "RZ_api.h"
#include "Log_api.h"

void mainLoop();

int main(void){

	VZ_TRACE("\nIniciando Render...");

	if(!renderz::init()) VZ_CRITICAL("\nRenderizador nao iniciou...");

	/* Loop until the user closes the window */
	mainLoop();
	
	/* Terminate GLFW*/
	renderz::terminate();

	VZ_TRACE("\nEnter para fechar...");
	getc(stdin);
	
	return 1;
}

void mainLoop() {
	bool keepGoing = true;
	while (keepGoing) {
		keepGoing = renderz::render();
	}
	VZ_INFO("\nJanela fechada...");
}