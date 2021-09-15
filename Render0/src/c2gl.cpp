#include "render0/c2gl.hpp"

namespace c2gl {

	c2glModelInfo_t* initModelInfo(mz::ModelZ* originalModel_ptr) {
		RZ_TRACE("Obtendo informacoes de modelo para renderer c2Gl...");
		modelInfo.originalModel_ptr = originalModel_ptr;
		
		modelInfo.numberTriangles = originalModel_ptr->getNumberTriangles();
		modelInfo.numberCurrentlyVisibleTriangles = modelInfo.numberTriangles;

		RZ_TRACE("Alocando memoria...");

		int sizeArraysPosAndNormal = modelInfo.numberTriangles * 3 * sizeof(glm::vec4);

		modelInfo.originalPositions = (glm::vec4*)malloc(sizeArraysPosAndNormal);
		modelInfo.originalNormals = (glm::vec4*)malloc(sizeArraysPosAndNormal);
		modelInfo.transformedPositions = (glm::vec4*)malloc(sizeArraysPosAndNormal);
		modelInfo.transformedNormals = (glm::vec4*)malloc(sizeArraysPosAndNormal);

		int sizeArrayFaceNormals = modelInfo.numberTriangles * sizeof(glm::vec4);
		modelInfo.originalFaceNormals = (glm::vec4*)malloc(sizeArrayFaceNormals);
		modelInfo.transformedFaceNormals = (glm::vec4*)malloc(sizeArrayFaceNormals);
		
		RZ_TRACE("Copiando informacoes de posicao e normal...");
		originalModel_ptr->copyPosBufferTo(modelInfo.numberTriangles,
			                               modelInfo.originalPositions);
		originalModel_ptr->copyNormBufferTo(modelInfo.numberTriangles,
										   modelInfo.originalNormals);
	
		originalModel_ptr->copyFaceNormalsTo(modelInfo.numberTriangles,
			                                 modelInfo.originalFaceNormals);
		
		RZ_TRACE("fazendo segunda copia, para transformacao...");
		originalModel_ptr->copyPosBufferTo(modelInfo.numberTriangles,
									       modelInfo.transformedPositions);
		originalModel_ptr->copyNormBufferTo(modelInfo.numberTriangles,
										   modelInfo.transformedNormals);
		
		originalModel_ptr->copyFaceNormalsTo(modelInfo.numberTriangles,
										     modelInfo.transformedFaceNormals);
		
		RZ_TRACE("Reiniciando numero de triangulos visiveis...");
		modelInfo.numberCurrentlyVisibleTriangles = modelInfo.numberTriangles;

		RZ_INFO("Dados copiados e memoria para manipulacao reservada");

		return &modelInfo;
	}

	//ESCREVER MATRIZ!
	glm::mat4 perspectiveProjection(float fovVert, float fovHor,
		float nearDist, float farDist) {

		return glm::perspective(fovVert, 640 / 480.f, fabs(nearDist), fabs(farDist));
	}

	glm::mat4 orthogonalProjection(float orthoDistance, float screenRatio,
		float nearDist, float farDist) {

		float top = orthoDistance;
		float bottom = -top;
		float right = top * screenRatio;
		float left = -right;
		float farD = farDist;
		float nearD = nearDist;

		return glm::ortho(left, right, bottom, top, nearD, farD);
	}

	void applyMVP(glm::mat4 viewProjMatrix, glm::mat4 modelMatrix) {
		glm::mat4 MVP = viewProjMatrix * modelMatrix;

		for (int i = 0; i < modelInfo.numberTriangles; i++) {
			modelInfo.transformedFaceNormals[i] =
				MVP * modelInfo.originalFaceNormals[i];

			for (int j = 0; j < 3; j++) {
				int index = (3 * i) + j;

				modelInfo.transformedPositions[index] =
					MVP * modelInfo.originalPositions[index];
			}
		}
	}

	bool isOutsideNearFar(renderArea_t renderArea, float z) {
		
		bool shouldCut = false;

		shouldCut |= (renderArea.nearDist > z );
		shouldCut |= (renderArea.farDist < z);

		return shouldCut;
	}

	void applyClippingOnZ(renderArea_t renderArea) {

		modelInfo.numberCurrentlyVisibleTriangles = modelInfo.numberTriangles;

		for (int i = 0; i < modelInfo.numberCurrentlyVisibleTriangles; i++) {
			bool shouldCut = false;

			for (int j = 0; j < 3; j++) {
				int index = (3 * i) + j;

				shouldCut |= isOutsideNearFar(renderArea, modelInfo.transformedPositions[index].z);
			}

			int cutCount = 0;

			while (shouldCut){
				cutCount++;

				shouldCut = false;
				for (int j = 0; j < 3; j++) {
					int index = ( 3 * (i + cutCount) ) + j;

					shouldCut |= isOutsideNearFar(renderArea, modelInfo.transformedPositions[index].z);
				}
			}

			modelInfo.transformedPositions[i] = modelInfo.transformedPositions[i + cutCount];
			modelInfo.transformedNormals[i] = modelInfo.transformedNormals[i + cutCount];
			modelInfo.numberCurrentlyVisibleTriangles -= cutCount;

			i += cutCount;
		}
	}

	void applyPerspectiveDivision() {
		for (int i = 0; i < modelInfo.numberCurrentlyVisibleTriangles; i++) {
			for (int j = 0; j < 3; j++) {
				int index = (3 * i) + j;

				modelInfo.transformedPositions[index] /=
					modelInfo.transformedPositions[index].w;
			}
		}
	}
}