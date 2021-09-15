#pragma once

/*
//Declara estruturas e classe de modelo.
//
//Dados do modelo:
//
//-Modelo tem um array de materiais, um de triangulos, tamanho de ambos e bounding box;
//	-Triangulos cada um com 3 vertexes + face normal;
//		--vertex com posicao, normal e indice de material;
//			---posicao e normal s�o glm::vec4 (mas leem 3 componentes)
//	-Material tem cor ambiente, difusa e especular e shine_coef
//		--cores s�o a princ�pio array de floats, depois mudar pra glm::vec4
//  -Bounding Box tem 2 v�rtices que englobam o modelo, o centro e a diagonal entre eles
//
//Al�m disso modelo cont�m buffers para uso pela gpu, que s�o um array para
//cada atributo, com valores para todos v�rtices. Tem que mudar isso pros
//materiais serem um array por material e adicionar buffer de �ndices de material.
//
//H� duplica��o de dados. Poderia deixar s� os buffers e criar m�todos de acesso
//pra outros usos. Al�m disso, n�o est� usando index arrays.
*/

#include "RZ_api.hpp"

#include "logAPI.hpp"


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

	typedef struct model_st {
		int numberTriangles;
		int numberMaterials;
		boundingBox_t bBox;

		triangle_t* triangles;
		material_t* materials;
	}model_t;

	class ModelZ {

	public:

		ModelZ(const char* model3DMaxFile);

		boundingBox_t getCopyBoundingBox();
		float getBoundingBoxDiagonal();
		glm::vec4 getBoundingBoxCenter();
		int ModelZ::getNumberTriangles();
		int ModelZ::getNumberMaterials();
		
		void ModelZ::copyPosBufferTo(int numberTriangles, glm::vec4* destinationTriangles_ptr);
		void ModelZ::copyNormBufferTo(int numberTriangles, glm::vec4* destinationTriangles_ptr);
		void ModelZ::copyFaceNormalsTo(int numberTriangles, glm::vec4* destinationTriangles_ptr);
		void ModelZ::copyNTrianglesTo(int numberTriangles, 
			                          triangle_t* destinationTriangles_ptr);
		
		void ModelZ::copyPosBufferFrom(int numberTriangles, glm::vec4* sourceTriangles_ptr);
		void ModelZ::copyNormBufferFrom(int numberTriangles, glm::vec4* sourceTriangles_ptr);
		void ModelZ::copyNTrianglesFrom(int numberTriangles,
										triangle_t* sourceTriangles_ptr);
	
		glm::vec4* posBuffer;
		glm::vec4* normBuffer;
		glm::vec4* ambientBuffer;
		glm::vec4* diffuseBuffer;
		glm::vec4* specularBuffer;
		float* spcShineCoefBuffer;

	private:

		void printModel();
		void printBuffers();

		model_t model;
	};
}