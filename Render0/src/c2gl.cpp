#include "render0/c2gl.hpp"

namespace c2gl {

	void initModelInfo(mz::ModelZ* originalModel_ptr) {
		RZ_TRACE("Obtendo informacoes de modelo para renderer c2Gl...");
		modelInfo.originalModel_ptr = originalModel_ptr;
		
		modelInfo.numberTriangles = originalModel_ptr->getNumberTriangles();
		int sizeTrianglesArray = modelInfo.numberTriangles * sizeof(mz::triangle_t);

		modelInfo.originalTringlesArray = (mz::triangle_t*)malloc(sizeTrianglesArray);
		modelInfo.transformedTringlesArray = (mz::triangle_t*)malloc(sizeTrianglesArray);
		
		originalModel_ptr->copyNTrianglesTo(modelInfo.numberTriangles, 
			                                modelInfo.originalTringlesArray);
		originalModel_ptr->copyNTrianglesTo(modelInfo.numberTriangles,
										    modelInfo.transformedTringlesArray);

		RZ_INFO("Dados copiados e memoria para manipulacao reservada");
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

		for (int i = 0; i < modelInfo.numberTriangles; i++) {
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
			modelInfo.numberTriangles -= cutCount;

			i += cutCount;
		}
	}

	void applyPerspectiveDivision() {
		for (int i = 0; i < modelInfo.numberTriangles; i++) {
			for (int j = 0; j < 3; j++) {
				modelInfo.transformedTringlesArray[i].vertexes[j].position /=
					modelInfo.transformedTringlesArray[i].vertexes[j].position.w;
			}
		}
	}
}