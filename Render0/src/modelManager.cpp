// Classe de modelo, declarada em model0.hpp 
// Carrega modelos .max e gera buffers a serem passados para placa de vídeo.
// Estrutura de dados tem que ser refeita (duplicações), e o construtor tá bem feio : )

#include "render0/model0.hpp"

using namespace mz;

boundingBox_t calculateBoundingBox(model_t model);

boundingBox_t ModelZ::getCopyBoundingBox() {
	return this->model.bBox;
}

ModelZ::ModelZ(const char* model3DMaxFile) {
	
	RZ_TRACE("\nLendo arquivo de modelo em formato .MAX");
	printf("\t( de %s )\n", model3DMaxFile);

	FILE* fp = fopen(model3DMaxFile, "r");
	if (fp == NULL) {
		RZ_CRITICAL("Nao conseguiu abrir o arquivo de modelo! Saindo...");
		printf("\t( arquivo: %s )\n", model3DMaxFile);
		getchar();
		exit(1);
	}

	RZ_INFO("Arquivo aberto...");

	char ch;
	fscanf(fp, "%c", &ch);
	while (ch != '\n') fscanf(fp, "%c", &ch); //consome primeira linha

	int materialCount;
	int triangleCount; 
	
	fscanf(fp, "# triangles = %d\n", &triangleCount);
	fscanf(fp, "Material count = %d\n", &materialCount);

	this->model.materials = (material_t*)malloc(materialCount * sizeof(material_t));
	this->model.triangles = (triangle_t*)malloc(triangleCount * sizeof(triangle_t));

	this->model.numberMaterials = materialCount;
	this->model.numberTriangles = triangleCount;
	
	RZ_INFO("Espaco alocado para modelo...");

	RZ_TRACE("\nLendo materiais");

	for (int i = 0; i < materialCount; i++) {

		fscanf(fp, "ambient color %f %f %f\n", &(this->model.materials[i].ambient[0])
				                             , &(this->model.materials[i].ambient[1])
			                                 , &(this->model.materials[i].ambient[2]));
		this->model.materials[i].ambient[3] = 1.f;

		fscanf(fp, "diffuse color %f %f %f\n", &(this->model.materials[i].diffuse[0])
			                                 , &(this->model.materials[i].diffuse[1])
									         , &(this->model.materials[i].diffuse[2]));
		this->model.materials[i].diffuse[3] = 1.f;

		fscanf(fp, "specular color %f %f %f\n", &(this->model.materials[i].specular[0])
			                                  , &(this->model.materials[i].specular[1])
			                                  , &(this->model.materials[i].specular[2]));
		this->model.materials[i].specular[3] = 1.f;
		
		fscanf(fp, "material shine %f\n", &(this->model.materials[i].specShineCoef));
	}

	fscanf(fp, "%c", &ch);
	while (ch != '\n') fscanf(fp, "%c", &ch); //consome linha documentação

	RZ_TRACE("\nLendo triangulos...");
	printf("\t( %d triangles ). . .\n", triangleCount);

	for (int i = 0; i < triangleCount; i++) {

		int vn = 0;
		fscanf(fp, "v0 %f %f %f %f %f %f %d\n",
			&(this->model.triangles[i].vertexes[vn].position.x),
			&(this->model.triangles[i].vertexes[vn].position.y),
			&(this->model.triangles[i].vertexes[vn].position.z),


			&(this->model.triangles[i].vertexes[vn].normal.x),
			&(this->model.triangles[i].vertexes[vn].normal.y),
			&(this->model.triangles[i].vertexes[vn].normal.z),

			&(this->model.triangles[i].vertexes[vn].materialIndex));

		this->model.triangles[i].vertexes[vn].position.w = 1.f;
		this->model.triangles[i].vertexes[vn].normal.w = 0.f;
		vn++;

		fscanf(fp, "v1 %f %f %f %f %f %f %d\n",
			&(this->model.triangles[i].vertexes[vn].position.x),
			&(this->model.triangles[i].vertexes[vn].position.y),
			&(this->model.triangles[i].vertexes[vn].position.z),


			&(this->model.triangles[i].vertexes[vn].normal.x),
			&(this->model.triangles[i].vertexes[vn].normal.y),
			&(this->model.triangles[i].vertexes[vn].normal.z),

			&(this->model.triangles[i].vertexes[vn].materialIndex));

		this->model.triangles[i].vertexes[vn].position.w = 1.f;
		this->model.triangles[i].vertexes[vn].normal.w = 0.f;
		vn++;

		fscanf(fp, "v2 %f %f %f %f %f %f %d\n",
			&(this->model.triangles[i].vertexes[vn].position.x),
			&(this->model.triangles[i].vertexes[vn].position.y),
			&(this->model.triangles[i].vertexes[vn].position.z),


			&(this->model.triangles[i].vertexes[vn].normal.x),
			&(this->model.triangles[i].vertexes[vn].normal.y),
			&(this->model.triangles[i].vertexes[vn].normal.z),

			&(this->model.triangles[i].vertexes[vn].materialIndex));

		this->model.triangles[i].vertexes[vn].position.w = 1.f;
		this->model.triangles[i].vertexes[vn].normal.w = 0.f;

		fscanf(fp, "face normal %f %f %f\n",
			&(this->model.triangles[i].faceNormal.x),
			&(this->model.triangles[i].faceNormal.y),
			&(this->model.triangles[i].faceNormal.z));
		this->model.triangles[i].faceNormal.w = 0.f;
	}
	fclose(fp);

	RZ_INFO("\nModelo lido!");

	RZ_TRACE("Determinando boundingBoxes e preparando buffers");
	this->model.bBox = calculateBoundingBox(this->model);

	//TESTE
	//this->printModel(); //TESTE
	//TESTE

	int numberVertices = this->model.numberTriangles * 3;

	this->posBuffer              = (glm::vec4*)malloc(numberVertices * sizeof(glm::vec4));
	this->normBuffer             = (glm::vec4*)malloc(numberVertices * sizeof(glm::vec4));
	

	//AQUI
	this->ambientBuffer          = (glm::vec4*)malloc(numberVertices * sizeof(glm::vec4));
	this->diffuseBuffer          = (glm::vec4*)malloc(numberVertices * sizeof(glm::vec4));
	this->specularBuffer         = (glm::vec4*)malloc(numberVertices * sizeof(glm::vec4));
	this->spcShineCoefBuffer     = (float*)malloc(numberVertices * sizeof(float));
	//AQUI


	RZ_TRACE("Espaco alocado, copiar dados...");

	for (int i = 0; i < this->model.numberTriangles; i++) {
		int index = 0;
		int matIndex = 0;
		for (int j = 0; j < 3; j++) {
			index = (3 * i) + j;

			this->posBuffer[index].x = this->model.triangles[i].vertexes[j].position.x;
			this->posBuffer[index].y = this->model.triangles[i].vertexes[j].position.y;
			this->posBuffer[index].z = this->model.triangles[i].vertexes[j].position.z;
			this->posBuffer[index].w = this->model.triangles[i].vertexes[j].position.w;

			this->normBuffer[index].x = this->model.triangles[i].vertexes[j].normal.x;
			this->normBuffer[index].y = this->model.triangles[i].vertexes[j].normal.y;
			this->normBuffer[index].z = this->model.triangles[i].vertexes[j].normal.z;
			this->normBuffer[index].w = this->model.triangles[i].vertexes[j].normal.w;
			

			//AQUI
			matIndex = this->model.triangles[i].vertexes[j].materialIndex;

			this->ambientBuffer[index].r = this->model.materials[matIndex].ambient[0];
			this->ambientBuffer[index].g = this->model.materials[matIndex].ambient[1];
			this->ambientBuffer[index].b = this->model.materials[matIndex].ambient[2];
			this->ambientBuffer[index].w = this->model.materials[matIndex].ambient[3];

			this->diffuseBuffer[index].r = this->model.materials[matIndex].diffuse[0];
			this->diffuseBuffer[index].g = this->model.materials[matIndex].diffuse[1];
			this->diffuseBuffer[index].b = this->model.materials[matIndex].diffuse[2];
			this->diffuseBuffer[index].w = this->model.materials[matIndex].diffuse[3];

			this->specularBuffer[index].r = this->model.materials[matIndex].specular[0];
			this->specularBuffer[index].g = this->model.materials[matIndex].specular[1];
			this->specularBuffer[index].b = this->model.materials[matIndex].specular[2];
			this->specularBuffer[index].w = this->model.materials[matIndex].specular[3];

			this->spcShineCoefBuffer[index] = this->model.materials[matIndex].specShineCoef;
			//AQUI
		}
	}

	//TESTE
	//this->printBuffers(); //TESTE
	//TESTE

	RZ_INFO("Buffers prontos!");

}

float ModelZ::getBoundingBoxDiagonal() {
	return this->model.bBox.diagonal;
}

glm::vec4 ModelZ::getBoundingBoxCenter() {
	return this->model.bBox.center;
}

int ModelZ::getNumberTriangles() {
	return this->model.numberTriangles;
}

int ModelZ::getNumberMaterials() {
	return this->model.numberMaterials;
}

void ModelZ::printModel() {
	RZ_WARN("\n\nPrintando modelo! Vai tocando enter pra ir passando (pode demorar)");

	printf("\n\nMaterials: %i\tTriangles: %i"
		  , this->model.numberMaterials, this->model.numberTriangles);
	
	printf("\n\nMaterial Info:");
	for (int i = 0; i < this->model.numberMaterials; i++) {
		printf("\n\n\tMaterial %i:",i);
		printf("\n\tAmbient: %f %f %f %f", this->model.materials[i].ambient[0]
			  , this->model.materials[i].ambient[1], this->model.materials[i].ambient[2]
			  , this->model.materials[i].ambient[3]);
		printf("\n\tDiffuse: %f %f %f %f", this->model.materials[i].diffuse[0]
			, this->model.materials[i].diffuse[1], this->model.materials[i].diffuse[2]
			, this->model.materials[i].diffuse[3]);
		printf("\n\tSpecular: %f %f %f %f", this->model.materials[i].specular[0]
			, this->model.materials[i].specular[1], this->model.materials[i].specular[2]
			, this->model.materials[i].specular[3]);
		printf("\n\tSpec Shine Coef: %f:", this->model.materials[i].specShineCoef);
	}

	getchar();

	printf("\n\nTriangles Info:");
	for (int i = 0; i < this->model.numberTriangles; i++) {
		printf("\n\tTriangle %i:", i);
		getchar();
		for (int j = 0; j < 3; j++) {
			printf("\n\t\tVertex %i:", j);

			printf("\n\tPositions: %f %f %f %f", this->model.triangles[i].vertexes[j].position.x
				                             , this->model.triangles[i].vertexes[j].position.y
				                             , this->model.triangles[i].vertexes[j].position.z
				                             , this->model.triangles[i].vertexes[j].position.w);
			printf("\n\tNormals: %f %f %f %f", this->model.triangles[i].vertexes[j].normal.x
				                            , this->model.triangles[i].vertexes[j].normal.y
				                            , this->model.triangles[i].vertexes[j].normal.z
				                            , this->model.triangles[i].vertexes[j].normal.w);
			printf("\n\tMaterial index: %i", this->model.triangles[i].vertexes[j].materialIndex);
		getchar();
		}
		printf("\n\tFace Normal: %f %f %f %f", this->model.triangles[i].faceNormal.x
			 								  , this->model.triangles[i].faceNormal.y
											  , this->model.triangles[i].faceNormal.z
											  , this->model.triangles[i].faceNormal.w);
		getchar();
	}

	printf("\n\nBounding Box:");
	printf("\n\txMin: %f, xMax: %f", this->model.bBox.minCoords.x
		                           , this->model.bBox.maxCoords.x);
	printf("\n\tyMin: %f, yMax: %f", this->model.bBox.minCoords.y
		                           , this->model.bBox.maxCoords.y);
	printf("\n\tzMin: %f, zMax: %f", this->model.bBox.minCoords.z
		                           , this->model.bBox.maxCoords.z);
	printf("\n\t\tCenter: %f %f %f %f", this->model.bBox.center.x
		                              , this->model.bBox.center.y
		                              , this->model.bBox.center.z
		                              , this->model.bBox.center.w);
	printf("\n\t\tDiagonal: %f\n\n", this->model.bBox.diagonal);

	RZ_WARN("\nFoi certo?");

	getchar();

}

void ModelZ::printBuffers() {
	
	RZ_WARN("\n\nPrintando buffers! Vai tocando enter pra ir passando (pode demorar)");

	printf("\n\n\tVertexes: %i, with 6 buffers each", this->model.numberTriangles*3);
	getchar();

	int index = 0;
	for (int i = 0; i < this->model.numberTriangles; i++) {
		for (int j = 0; j < 3; j++) {
			index = (3 * i) + j;
			
			printf("\n\nTriangulo %i, Vertice %i", i, j);

			printf("\n\tPos: \t%f, %f, %f, %f", this->posBuffer[index].x,
				                                this->posBuffer[index].y,
				                                this->posBuffer[index].z,
				                                this->posBuffer[index].w);

			printf("\n\tNorm: \t%f, %f, %f, %f", this->normBuffer[index].x,
												 this->normBuffer[index].y,
												 this->normBuffer[index].z,
												 this->normBuffer[index].w);

			printf("\n\tAmbient: \t%f, %f, %f, %f", this->ambientBuffer[index].x,
													this->ambientBuffer[index].y,
													this->ambientBuffer[index].z,
													this->ambientBuffer[index].w);

			printf("\n\tDiffuse: \t%f, %f, %f, %f", this->diffuseBuffer[index].x,
													this->diffuseBuffer[index].y,
													this->diffuseBuffer[index].z,
													this->diffuseBuffer[index].w);

			printf("\n\tSpecular: \t%f, %f, %f, %f", this->specularBuffer[index].x,
  													 this->specularBuffer[index].y,
													 this->specularBuffer[index].z,
													 this->specularBuffer[index].w);
			
			printf("\n\tSpecular Shine Coeficient: %f", this->spcShineCoefBuffer[index]);
			
			getchar();
		}
	}

	RZ_WARN("\n\nPrintados os buffers! Enter pra continuar...");

	getchar();
}

boundingBox_t calculateBoundingBox(model_t model) {
	RZ_TRACE("Calculando Bounding Box");

	float xMin = model.triangles[0].vertexes[0].position.x;
	float xMax = model.triangles[0].vertexes[0].position.x;
	float yMin = model.triangles[0].vertexes[0].position.y;
	float yMax = model.triangles[0].vertexes[0].position.y;
	float zMin = model.triangles[0].vertexes[0].position.z;
	float zMax = model.triangles[0].vertexes[0].position.z;

	for (int i = 0; i < model.numberTriangles; i++) {
		for (int j = 0; j < 3; j++) {
			
			if (model.triangles[i].vertexes[j].position.x < xMin)
				xMin = model.triangles[i].vertexes[j].position.x;
			else if (model.triangles[i].vertexes[j].position.x > xMax)
				xMax = model.triangles[i].vertexes[j].position.x;

			if (model.triangles[i].vertexes[j].position.y < yMin)
				yMin = model.triangles[i].vertexes[j].position.y;
			else if (model.triangles[i].vertexes[j].position.y > yMax)
				yMax = model.triangles[i].vertexes[j].position.y;

			if (model.triangles[i].vertexes[j].position.z < zMin)
				zMin = model.triangles[i].vertexes[j].position.z;
			else if (model.triangles[i].vertexes[j].position.z > zMax)
				zMax = model.triangles[i].vertexes[j].position.z;
		}	
	}

	boundingBox_t bBox;

	bBox.minCoords = glm::vec4(xMin, yMin, zMin, 1.f);
	bBox.maxCoords = glm::vec4(xMax, yMax, zMax, 1.f);

	RZ_TRACE("Pronto, calculando centro e diagonal");

	bBox.center = (bBox.minCoords + bBox.maxCoords) / 2.f;

	bBox.diagonal = sqrtf(  (xMax - xMin) * (xMax - xMin)
		                  + (yMax - yMin) * (yMax - yMin)
				          + (zMax - zMin) * (zMax - zMin) );

	return bBox;
}