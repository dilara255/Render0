#pragma once

/*
//Declara estruturas e funções para renderização por software,
//eventualmente emulando um subconjunto significativo do pipeline
//gráfico, baseado em openGL.
//
//Organizado para ser utilizado de forma permutável com o pipeline normal 
//através do renderer aqui implementado.
//
//Guarda referência para modelo original, cria uma cópia das informações 
//de triângulos e reserva espaço para as informações modificadas para renderização,
//que são autalizadas sempre que há alteração de câmera ou de matriz de modelo.
//   *a ideia é aumentar compatibilidade entre sistemas de render e 
//   localidade de memória, mas há duplicação de informação para tanto. Tem que testar.
//
//Funcionalidade por enquanto é:
//
// - Criar matrizes de View e Projection;
// - Aplicar matriz MVP no modelo, gravando as novas POSIÇÕES dos vertex 
//   (por enqunato não modifica normais. Buffer próprio não contém materiais);
// - Descartar triângulos com vértices com "z" no espaço de câmera fora do near/far;
// - Fazer divisao perspectiva;
// - Enviar vertexes para renderização pela placa de vídeo (usando opengl);
// 
// Futuramente mais passos da pipeline gráfica serão feitos em software, 
// como o clipping nas bordas da viewport e a rasterização.
*/

#include <glm/gtc/matrix_transform.hpp>

#include "RZ_API/RZ_api.hpp"

#include "render0/model0.hpp"
#include "render0/camera.hpp"

namespace c2gl {

	typedef struct c2glModelInfo_st {
		mz::ModelZ* originalModel_ptr;

		glm::vec4* originalPositions;
		glm::vec4* originalNormals;
		glm::vec4* originalFaceNormals;
		glm::vec4* transformedPositions;
		glm::vec4* transformedNormals;
		glm::vec4* transformedFaceNormals;

		int numberTriangles;
		int numberCurrentlyVisibleTriangles;
	} c2glModelInfo_t;

	static c2glModelInfo_t modelInfo;

	c2glModelInfo_t* initModelInfo(mz::ModelZ* originalModel_ptr);

	void prerender(CameraZ* cam_ptr, c2glModelInfo_t* modelInfo_ptr);

	glm::mat4 perspectiveProjection(float fovVert, float fovHor,
			                        float nearDist, float farDist);
		
	glm::mat4 orthogonalProjection(float top, float right,
								   float nearDist, float farDist);

	glm::mat4 constructViewProjectionMatrix(CameraZ cam);

	void applyMVP(glm::mat4 viewProjMatrix, glm::mat4 modelMatrix, 
		          c2glModelInfo_t* modelInfo_ptr);
	
	void applyClippingOnZ(renderArea_t renderArea,
		                  c2glModelInfo_t* modelInfo_ptr);

	void applyPerspectiveDivision(c2glModelInfo_t* modelInfo_ptr);
}