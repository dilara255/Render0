#pragma once

//"Ponto de acesso" à dll e ao funcionamento exposto de cada "subsistema"

#include "core.hpp"
#include "miscStdHeaders.h"

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

namespace mz {
	class ModelZ;

	typedef struct boundingBox_st {
		glm::vec4 minCoords;
		glm::vec4 maxCoords;

		glm::vec4 center;
		float diagonal;
	}boundingBox_t;
}