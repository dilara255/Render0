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

	glm::mat4 orthogonalProjection(float top, float right,
								   float nearDist, float farDist) {

		float bottom = -top;
		float left = -right;

		return glm::ortho(left, right, bottom, top, nearDist, farDist);
	}

	void applyMVP(glm::mat4 viewProjMatrix, glm::mat4 modelMatrix, 
				  c2glModelInfo_t* modelInfo_ptr) {

		glm::mat4 MVP = viewProjMatrix * modelMatrix;

		for (int i = 0; i < modelInfo_ptr->numberTriangles; i++) {
			modelInfo_ptr->transformedFaceNormals[i] =
				MVP * modelInfo_ptr->originalFaceNormals[i];

			for (int j = 0; j < 3; j++) {
				int index = (3 * i) + j;

				modelInfo_ptr->transformedPositions[index] =
					MVP * modelInfo_ptr->originalPositions[index];
			}
		}
	}

	bool isOutsideNearFar(renderArea_t renderArea, float z) {
		
		bool shouldCut = false;

		shouldCut |= (renderArea.nearDist > z );
		shouldCut |= (renderArea.farDist < z);

		return shouldCut;
	}

	void applyClippingOnZ(renderArea_t renderArea, c2glModelInfo_t* modelInfo_ptr) {

		modelInfo_ptr->numberCurrentlyVisibleTriangles = modelInfo_ptr->numberTriangles;

		for (int i = 0; i < modelInfo_ptr->numberCurrentlyVisibleTriangles; i++) {
			bool shouldCut = false;

			for (int j = 0; j < 3; j++) {
				int index = (3 * i) + j;

				shouldCut |= isOutsideNearFar(renderArea, modelInfo_ptr->transformedPositions[index].z);
			}

			int cutCount = 0;

			while (shouldCut){
				cutCount++;

				shouldCut = false;
				for (int j = 0; j < 3; j++) {
					int index = ( 3 * (i + cutCount) ) + j;

					shouldCut |= isOutsideNearFar(renderArea, modelInfo_ptr->transformedPositions[index].z);
				}
			}

			modelInfo_ptr->transformedPositions[i] = modelInfo_ptr->transformedPositions[i + cutCount];
			modelInfo_ptr->transformedNormals[i] = modelInfo_ptr->transformedNormals[i + cutCount];
			modelInfo_ptr->numberCurrentlyVisibleTriangles -= cutCount;

			i += cutCount;
		}
	}

	void applyPerspectiveDivision(c2glModelInfo_t* modelInfo_ptr) {
		for (int i = 0; i < modelInfo_ptr->numberCurrentlyVisibleTriangles; i++) {
			for (int j = 0; j < 3; j++) {
				int index = (3 * i) + j;

				modelInfo_ptr->transformedPositions[index] /=
					modelInfo_ptr->transformedPositions[index].w;
			}
		}
	}

	glm::mat4 constructViewProjectionMatrix(CameraZ cam) {
		glm::mat4 viewProj;

		if (cam.perspectiveIsOn()) {
			viewProj = perspectiveProjection(cam.getFovVer(), cam.getFovHor(),
				cam.getNearDist(), cam.getFarDist());
		}
		else {
			renderArea_st renderArea = cam.getRenderArea();
			viewProj = orthogonalProjection(renderArea.top, renderArea.right,
										    renderArea.nearDist, renderArea.farDist);
		}

		return (viewProj * cam.getViewMatrix());
	}

	void prerender(CameraZ* cam_ptr, c2glModelInfo_t* modelInfo_ptr) {
		glm::mat4 viewProj = constructViewProjectionMatrix(*cam_ptr);
		
		applyMVP(viewProj, glm::mat4(1), modelInfo_ptr);
		applyClippingOnZ(cam_ptr->getRenderArea(), modelInfo_ptr);
		applyPerspectiveDivision(modelInfo_ptr);
	}
}