#include "RZ_api.hpp"
#include "logAPI.hpp"

void loop1();

int main(void){

	VZ_TRACE("\nIniciando Render...");

	if (!rz::init()) VZ_CRITICAL("\nRenderizador nao iniciou...");

	/* Loop until the user closes the window */
	loop1();

	VZ_TRACE("Readquirir contexto...");

	if (!rz::init()) VZ_CRITICAL("\nContexto nao voltou!");

	VZ_INFO("Contexto Readquirido...");

	/* Loop until the user closes the window */
	rz::testTriangle();
	VZ_INFO("\nJanela fechada...");

	VZ_TRACE("\nEnter para fechar...");
	getc(stdin);
	
	return 1;
}

void loop1() {
	bool keepGoing = true;
	while (keepGoing) {
		keepGoing = rz::renderTest0();
	}
	VZ_INFO("\nJanela fechada...");

	/* Terminate GLFW*/
	rz::terminate();
}