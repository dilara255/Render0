#pragma once

/*
//Declara estruturas e fun��es para renderiza��o por software,
//eventualmente emulando um subconjunto significativo do pipeline
//gr�fico, baseado em openGL.
//
//Organizado para ser utilizado de forma permut�vel com o pipeline normal 
//atrav�s do renderer aqui implementado.
//
//Guarda refer�ncia para modelo original, cria uma c�pia das informa��es 
//de tri�ngulos e reserva espa�o para as informa��es modificadas para renderiza��o,
//que s�o autalizadas sempre que h� altera��o de c�mera ou de matriz de modelo.
//   *a ideia � aumentar compatibilidade entre sistemas de render e 
//   localidade de mem�ria, mas h� duplica��o de informa��o para tanto. Tem que testar.
//
//Funcionalidade por enquanto �:
//
// - Criar matrizes de View e Projection;
// - Aplicar matriz MVP no modelo, gravando as novas POSI��ES dos vertex 
//   (por enqunato n�o modifica normais. Buffer pr�prio n�o cont�m materiais);
// - Descartar tri�ngulos com v�rtices com "z" no espa�o de c�mera fora do near/far;
// - Fazer divisao perspectiva;
// - Enviar vertexes para renderiza��o pela placa de v�deo (usando opengl);
// 
// Futuramente mais passos da pipeline gr�fica ser�o feitos em software, 
// como o clipping nas bordas da viewport e a rasteriza��o.
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