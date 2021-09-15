#include "render0/c2gl.hpp"

namespace c2gl {

	c2glModelInfo_t* initModelInfo(mz::ModelZ* originalModel_ptr) {
		RZ_TRACE("Obtendo informacoes de modelo para renderer c2Gl...");
		modelInfo.originalModel_ptr = originalModel_ptr;
		
		modelInfo.numberTriangles = originalModel_ptr->getNumberTriangles();
		modelInfo.numberCurrentlyVisibleTriangles = modelInfo.numberTriangles;

		int sizeTrianglesArray = modelInfo.numberTriangles * sizeof(mz::triangle_t);

		RZ_TRACE("Alocando memoria...");

		modelInfo.originalTringlesArray = (mz::triangle_t*)malloc(sizeTrianglesArray);
		modelInfo.transformedTringlesArray = (mz::triangle_t*)malloc(sizeTrianglesArray);
		
		RZ_TRACE("Copiando informacoes de posicao e normal...");
		originalModel_ptr->copyNTrianglesTo(modelInfo.numberTriangles, 
			                                modelInfo.originalTringlesArray);
		RZ_TRACE("fazendo segunda copia, para transformacao...");
		originalModel_ptr->copyNTrianglesTo(modelInfo.numberTriangles,
										    modelInfo.transformedTringlesArray);

		RZ_TRACE("Atualizando numero de triangulos...");
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
			modelInfo.transformedTringlesArray[i].faceNormal =
				MVP * modelInfo.originalTringlesArray[i].faceNormal;

			for (int j = 0; j < 3; j++) {
				modelInfo.transformedTringlesArray[i].vertexes[j].position =
					MVP * modelInfo.originalTringlesArray[i].vertexes[j].position;
			}
		}
	}

	void applyClippingOnZ(renderArea_t renderArea) {

		modelInfo.numberCurrentlyVisibleTriangles = modelInfo.numberTriangles;

		for (int i = 0; i < modelInfo.numberCurrentlyVisibleTriangles; i++) {
			bool shouldCut = false;

			for (int j = 0; j < 3; j++) {
				shouldCut |= (renderArea.nearDist > 
						      modelInfo.transformedTringlesArray[i].vertexes[j].position.z);
				shouldCut |= (renderArea.farDist <
					          modelInfo.transformedTringlesArray[i].vertexes[j].position.z);
			}

			int cutCount = 0;

			while (shouldCut){
				cutCount++;

				shouldCut = false;
				for (int j = 0; j < 3; j++) {
					shouldCut |= (renderArea.nearDist >
						modelInfo.transformedTringlesArray[i+ cutCount].vertexes[j].position.z);
					shouldCut |= (renderArea.farDist <
						modelInfo.transformedTringlesArray[i+ cutCount].vertexes[j].position.z);
				}
			}

			modelInfo.transformedTringlesArray[i] = modelInfo.transformedTringlesArray[i + cutCount];
			modelInfo.numberCurrentlyVisibleTriangles -= cutCount;

			i += cutCount;
		}
	}

	void applyPerspectiveDivision() {
		for (int i = 0; i < modelInfo.numberCurrentlyVisibleTriangles; i++) {
			for (int j = 0; j < 3; j++) {
				modelInfo.transformedTringlesArray[i].vertexes[j].position /=
					modelInfo.transformedTringlesArray[i].vertexes[j].position.w;
			}
		}
	}
}