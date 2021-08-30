#pragma once

#include "core.hpp"

typedef struct renderInfo_st renderInfo_t;

namespace rz {

	RZ_API int init(void);
	RZ_API int renderTestLegacy(void);
	RZ_API int testTriangle(void);
	RZ_API int testTriangleWithCamera(void);
	RZ_API int modelMaterialRenderTest(const char* modelFile);
	RZ_API void terminate(void);

	typedef float colorf_t[4];
}