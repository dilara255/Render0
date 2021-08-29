#pragma once

#include "miscStdHeaders.h"

#include "RZ_api.hpp"
#include "logAPI.hpp"

/*
//Dados do modelo:
// 
//-Modelo tem um array de materiais, um de triangulos, e tamanho de ambos;
//	-Triangulos cada um com 3 vertexes + face normal;
//		--vertex com posicao, normal e indice de material;
//			---posicao e normal são glm::vec4 (mas leem 3 componentes)
//	-Material tem cor ambiente, difusa e especular e shine_coef
//		--cores são a princípio array de floats, depois mudar pra glm::vec4
*/

namespace mz {
	typedef struct material_st {
		rz::colorf_t ambient;
		rz::colorf_t diffuse;
		rz::colorf_t specular;
		float specShineCoef;
	}material_t;

	typedef struct vertex_st {
		glm::vec4 position;
		glm::vec4 normal;
		int materialIndex;
	}vertex_t;

	typedef struct triangle_st {
		vertex_t vertexes[3];
		glm::vec4 faceNormal;
	}triangle_t;

	typedef struct boundingBox_st {
		glm::vec4 minCoords;
		glm::vec4 maxCoords;

		glm::vec4 center;
		float diagonal;
	}boundingBox_t;

	typedef struct model_st {
		int numberTriangles;
		int numberMaterials;
		boundingBox_t bBox;

		triangle_t* triangles;
		material_t* materials;
	}model_t;

	class ModelZ {

	public:

		ModelZ(model_t model);
		ModelZ(char* model3DMaxFile);

		float getBoundingBoxDiagonal();
		glm::vec4 getBoundingBoxCenter();

	private:

		void printModel();

		model_t model;
	};
}