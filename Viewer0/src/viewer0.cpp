//Ponto de iício do programa. Ideia é passar funcionalidade de "modelRenderTest" pra
//esse projeto aqui. Controles de GUI e etc também serão colocados nele. 

#include "RZ_api.hpp"
#include "logAPI.hpp"

void legacyTestLoop();

int main(void){

	VZ_TRACE("\nIniciando Render...");

	if (!rz::init()) VZ_CRITICAL("\nRenderizador nao iniciou...");
	
	//rz::testTriangleWithCamera();
	rz::modelMaterialRenderTest("models/cow.max");

	VZ_TRACE("\nEnter para sair...");
	getchar();
	
	return 1;
}

void legacyTestLoop() {
	bool keepGoing = true;
	while (keepGoing) {
		keepGoing = rz::renderTestLegacy();
	}
	VZ_INFO("\nJanela fechada...");

	/* Terminate GLFW*/
	rz::terminate();
}