#include "RZ_api.h"

namespace renderz {
	__declspec(dllimport) int render(void);
}

int main(void){

	renderz::render();
	
	return 1;
}