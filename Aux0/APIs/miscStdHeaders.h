#pragma once

//Bibliotecas de uso geral. Précompiladas em projeto Aux0, que gera biblioteca estática

#ifdef RZ_PLATFORM_WINDOWS
	#include <windows.h>
#endif

#include <iostream>
#include <memory>
#include <cstdlib>

#include "glm/glm.hpp"

typedef glm::float32 radians;
