//Classe de câmera, declarada em camera.hpp
//Suporta rotação, translação, projeção perspectiva e ortogonal e lookat
//Usa coordenadas com vetores de base "view", "up" e "side".
//Não há cuidado para tamanho de passo (alguñs métodos podem bugar ocm passos grandes)
//Mantém vetores de base ortonormais
//Guarda estado inicial para poder resetar

#include "render0/camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

CameraZ::CameraZ(cameraState_t initialState) {
	this->state.lookAtActive = initialState.lookAtActive;
	this->state.perspectiveOn = initialState.perspectiveOn;

	this->state.position = initialState.position;

	this->state.lookatPosition = initialState.lookatPosition;
	this->state.viewDirection = initialState.viewDirection;
	this->state.upDirection = initialState.upDirection;
	this->state.sideDirection = initialState.sideDirection;

	this->state.orthoDistance = initialState.orthoDistance;
	this->state.nearDist = initialState.nearDist;
	this->state.farDist = initialState.farDist;
	this->state.fovHor = initialState.fovHor;
	this->state.fovVert = initialState.fovVert;

	this->state.cameraMatrix = initialState.cameraMatrix;
	this->state.view = initialState.view;
	this->state.perspective = initialState.perspective;

	this->cameraProjection = this->state.cameraMatrix;

	this->initialState = this->state;

	screenRatio = DEFAULT_WIDTH / (float)DEFAULT_HEIGHT;
}

CameraZ CameraZ::getCameraCenteredOnModel(mz::ModelZ* model_ptr, mz::boundingBox_t bBox,
	                                      bool lookAt, bool perspective,
	                                      float fov, float nearDist, float farDist) {

	RZ_TRACE("Inicializando camera...");

	cameraState_t initialState;

	initialState.lookAtActive = lookAt;
	initialState.perspectiveOn = perspective;

	float bBoxDiagonal = bBox.diagonal;
	float cx = bBox.center.x;
	float cy = bBox.center.y;
	float cz = bBox.center.z;

	initialState.nearDist = nearDist;
	initialState.farDist = farDist;
	initialState.fovHor = glm::radians(fov);
	initialState.fovVert = glm::radians(fov);
	initialState.orthoDistance = bBoxDiagonal / 2.f;

	float distanceToFitModel = bBoxDiagonal / (2 * tanf(initialState.fovVert / 2.f));

	printf("\n\nDistancia: %f", distanceToFitModel);

	initialState.position = glm::vec3(cx, cy, distanceToFitModel);

	initialState.lookatPosition = glm::vec3(cx, cy, cz);
	initialState.viewDirection = glm::vec3(0.0f, 0.0f, -1);
	initialState.upDirection = glm::vec3(0, 1, 0);
	initialState.sideDirection = glm::vec3(1, 0, 0);

	CameraZ camera(initialState);

	camera.updateCameraMatrix();

	RZ_INFO("Camera iniciada!");

	return camera;
}

void CameraZ::translateWorldCoord(glm::vec3 translation) {
	this->state.position += glm::vec3(translation);
}

void CameraZ::translateCameraCoord(glm::vec3 translation) {
	
	this->state.position += this->state.viewDirection * translation.z;
	this->state.position += this->state.upDirection * translation.y;
	this->state.position += this->state.sideDirection * translation.x;

	//se pá vetores up e side tão ficando loco tb quando chega rodopiando
	if (this->lookAtIsOn()) {
		float distanceToLookAt = glm::length(  this->state.position 
			                                 - this->state.lookatPosition);
		if (fabs(distanceToLookAt) < MIN_NEAR) {

			RZ_WARN("MUITO PERTO!");
			printf("\nPosition\t%f, %f, %f", this->state.position.x
				, this->state.position.y
				, this->state.position.z);
			printf("\nView: \t%f, %f, %f", this->state.viewDirection.x
				, this->state.viewDirection.y
				, this->state.viewDirection.z);

			if (this->perspectiveIsOn()) {
				this->state.position = this->state.lookatPosition
					                 + (2.f*this->state.viewDirection);
			} 

			printf("\nMais longe? \t%f, %f, %f", this->state.position.x
				                               , this->state.position.y
				                               , this->state.position.z);
			printf("\nView: \t%f, %f, %f", this->state.viewDirection.x
				                       , this->state.viewDirection.y
				                       , this->state.viewDirection.z);
			RZ_WARN("Tentou se recuperar, mas isso tá em teste ainda : /");
		}
	}
}

glm::vec3 CameraZ::getPosition() {
	return glm::vec3(this->state.position.x
		           , this->state.position.y, this->state.position.z);
}

void CameraZ::roll(radians angle){
	this->rotate(angle, &this->state.upDirection, &this->state.sideDirection);
}
void CameraZ::yaw(radians angle) {
	this->rotate(angle, &this->state.viewDirection, &this->state.sideDirection);
}
void CameraZ::pitch(radians angle) {
	this->rotate(angle, &this->state.viewDirection, &this->state.upDirection);
}

glm::vec3 CameraZ::getView() {
	return glm::vec3(this->state.viewDirection.x
		, this->state.viewDirection.y, this->state.viewDirection.z);
}

glm::vec3 CameraZ::getLookAtPos() {
	return glm::vec3(this->state.lookatPosition);
}

void CameraZ::rotate(radians angle, glm::vec3* directionFrom, glm::vec3* directionTo){
//rotaciona em torno do eixo perpendicular aos vetores dados. Efeito é como se "girasse"
//um dos vetores na direção do outro (e o outro junto). Equivalente às operações em uma
//matriz de rotação.

	glm::vec3 tempDirectionFrom = *directionFrom;
	glm::float32 sineAngle = glm::sin(angle);
	glm::float32 cosineAngle = glm::cos(angle);

	*directionFrom = glm::normalize(  cosineAngle * (*directionFrom)
				                    + sineAngle * (*directionTo) );

	*directionTo = glm::normalize( (-sineAngle) * tempDirectionFrom
				                   + cosineAngle * (*directionTo) );	
}

void CameraZ::lookAtToggle() {
	this->state.lookAtActive = !this->state.lookAtActive;
}
void CameraZ::lookAtTurnOn() {
	this->state.lookAtActive = true;
}
void CameraZ::lookAtTurnOff() {
	this->state.lookAtActive = false;
}
bool CameraZ::lookAtIsOn() {
	return this->state.lookAtActive;
}
void CameraZ::lookAtSetPos(glm::vec3 position) {
	this->state.lookatPosition = position;
}

void CameraZ::perspectiveToggle() {
	if(this->state.perspectiveOn)  perspectiveTurnOff();
	else perspectiveTurnOn();
}
void CameraZ::perspectiveTurnOn() {
	this->state.perspectiveOn = true;
	this->state.viewDirection *= -1;
}
void CameraZ::perspectiveTurnOff() {
	this->state.perspectiveOn = false;
	this->state.viewDirection *= -1;
}
bool CameraZ::perspectiveIsOn() {
	return this->state.perspectiveOn;
}

glm::mat4 CameraZ::getCameraMatrix() {
	return this->state.cameraMatrix;
}

radians CameraZ::getNearDist() {
	return this->state.nearDist;
}
radians CameraZ::getFarDist() {
	return this->state.farDist;
}

void CameraZ::changeNearDistBy(glm::float32 nearDistChange) {
	int sign = 1;
	if (this->state.nearDist < 0) sign = -1;

	this->state.nearDist += (sign * nearDistChange);

	if ( ( (sign < 0) && (this->state.nearDist > (-MIN_NEAR) ) )
	   ||( (sign > 0) && (this->state.nearDist <   MIN_NEAR) ) ) {

		this->state.nearDist = sign * MIN_NEAR;
	} else if (fabs(this->state.nearDist) >= fabs(this->state.farDist))
		this->state.nearDist = this->state.farDist - (sign * MIN_NEAR);
}
void CameraZ::changeFarDistBy(glm::float32 farDistChange) {
	int sign = 1;
	if (this->state.farDist < 0) sign = -1;

	this->state.farDist += (sign * farDistChange);

	if (((sign < 0) && (this->state.farDist < (-MAX_FAR)))
		|| ((sign > 0) && (this->state.farDist > MAX_FAR))) {

		this->state.farDist = sign * MAX_FAR;
	}
	else if (fabs(this->state.farDist) <= fabs(this->state.nearDist))
		this->state.farDist = this->state.nearDist + (sign * MIN_NEAR);
}

radians CameraZ::getFovHor() {
	return this->state.fovHor;
}
radians CameraZ::getFovVer() {
	return this->state.fovVert;
}

void CameraZ::setFovHor(radians fovHor) {
	if (fovHor > glm::radians(180.f)) fovHor = glm::radians(180.f);
	else if (fovHor < glm::radians(0.1f)) fovHor = glm::radians(0.1f);

	this->state.fovHor = fovHor;
	this->state.orthoDistance = fabs(sinf(fovHor/2.f)* this->state.farDist / 2.0f);
}
void CameraZ::setFovVer(radians fovVert) {
	if (fovVert > glm::radians(180.f)) fovVert = glm::radians(180.f);
	else if (fovVert < glm::radians(0.1f)) fovVert = glm::radians(0.1f);

	this->state.fovVert = fovVert;
	this->state.orthoDistance = fabs(sinf(fovVert/2.f)* this->state.farDist / 2.0f);
}

void CameraZ::resetCamera() {
	this->state = this->initialState;
}

void CameraZ::updateViewToLookat() {
	this->state.viewDirection = this->state.lookatPosition - this->state.position;
	if(this->perspectiveIsOn()) this->state.viewDirection *= -1;
	this->state.viewDirection /= glm::length(this->state.viewDirection);

	this->state.upDirection -= this->state.viewDirection
		                     * glm::dot( this->state.upDirection
								       , this->state.viewDirection);
	this->state.upDirection /= glm::length(this->state.upDirection);

	this->state.sideDirection -= this->state.viewDirection
						       * glm::dot( this->state.sideDirection
							             , this->state.viewDirection);
	this->state.sideDirection /= glm::length(this->state.sideDirection);
}

void CameraZ::updateViewMatrix() {
	if (this->state.lookAtActive) this->updateViewToLookat();
	
	/*translação origem mundo(0) - origem câmera
	* T = [-cx, -cy, -cz]
	*/

	this->state.view = glm::mat4( glm::vec4(1, 0, 0, 0)
							    , glm::vec4(0, 1, 0, 0)
								, glm::vec4(0, 0, 1, 0)
								, glm::vec4( -this->state.position.x
									       , -this->state.position.y
									       , -this->state.position.z, 1.f));

	/*
	* aí depois tem que mudar da base do mundo pra base da camera 
	(caso coords camera se mantenham paralelas as do mundo, não muda nada, certo?)
	*/

	glm::mat4 cameraCoordsTransf( glm::vec4( this->state.sideDirection.x
										   , this->state.upDirection.x
										   , this->state.viewDirection.x, 0 )
								, glm::vec4( this->state.sideDirection.y
									       , this->state.upDirection.y
									       , this->state.viewDirection.y, 0 )
								, glm::vec4( this->state.sideDirection.z
										   , this->state.upDirection.z
										   , this->state.viewDirection.z, 0 )
								, glm::vec4( 0, 0, 0, 1) );
	
	this->state.view = cameraCoordsTransf * this->state.view;
}

void CameraZ::updatePerspectiveMatrix() {
	if (this->state.perspectiveOn) {

		this->state.perspective = glm::perspective( this->state.fovVert
			                                      , screenRatio
			                                      , fabs(this->state.nearDist)
												  , fabs(this->state.farDist) );
	}

	else {
		float top = this->state.orthoDistance;
		float bottom = -top;
		float right = top * screenRatio;
		float left = -right;
		float farD = this->state.farDist;
		float nearD = this->state.nearDist;

		this->state.perspective = glm::ortho(left, right, bottom, top, nearD, farD);
	}
}

void CameraZ::updateCameraMatrix() {
	this->updateViewMatrix();
	this->updatePerspectiveMatrix();
	this->state.cameraMatrix = this->state.perspective * this->state.view;
	this->cameraProjection = this->state.cameraMatrix;
}