#include "render0/camera.hpp"

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

	screenRatio = DEFAULT_WIDTH / (float)DEFAULT_HEIGHT;
}

void CameraZ::translateWorldCoord(glm::vec3 translation) {
	this->state.position += glm::vec3(translation);
}
void CameraZ::translateCameraCoord(glm::vec3 translation) {
	this->state.position += this->state.viewDirection * translation.z;
	this->state.position += this->state.upDirection * translation.y;
	this->state.position += this->state.sideDirection * translation.x;
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

void CameraZ::rotate(radians angle, glm::vec3* directionFrom, glm::vec3* directionTo){
//eventualmente usar versão em matriz né, mas por enquanto vai isso:

	glm::vec3 tempDirectionFrom = *directionFrom;
	glm::float32 sineAngle = glm::sin(angle);
	glm::float32 cosineAngle = glm::cos(angle);

	*directionFrom = cosineAngle * (*directionFrom)
				   + sineAngle * (*directionTo);

	*directionTo = (-sineAngle) * tempDirectionFrom;
				 + cosineAngle * (*directionTo);	
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
	this->state.lookatPosition = glm::vec4(position,0.f);
}

void CameraZ::perspectiveToggle() {
	this->state.perspectiveOn = !this->state.perspectiveOn;
}
void CameraZ::perspectiveTurnOn() {
	this->state.perspectiveOn = true;
}
void CameraZ::perspectiveTurnOff() {
	this->state.perspectiveOn = false;
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

void CameraZ::setNearDist(glm::float32 nearDist) {
	this->state.nearDist = nearDist;
}
void CameraZ::setFarDist(glm::float32 farDist) {
	this->state.farDist = farDist;
}

radians CameraZ::getFovHor() {
	return this->state.fovHor;
}
radians CameraZ::getFovVer() {
	return this->state.fovVert;
}

void CameraZ::setFovHor(radians fovHor) {
	this->state.fovHor = fovHor;
}
void CameraZ::setFovVer(radians fovVert) {
	this->state.fovVert = fovVert;
}

void CameraZ::updateViewToLookat() {
	this->state.viewDirection = this->state.lookatPosition - this->state.position;
	this->state.viewDirection /= glm::length(this->state.viewDirection);
}

void CameraZ::updateViewMatrix() {
	if (this->state.lookAtActive) this->updateViewToLookat();
	
	glm::float32 sideDotPos = glm::dot(this->state.sideDirection, this->state.position);
	glm::float32 upDotPos = glm::dot(this->state.upDirection, this->state.position);
	glm::float32 viewDotPos = glm::dot(this->state.viewDirection, this->state.position);

	this->state.view = glm::mat4( glm::vec4(this->state.sideDirection, -sideDotPos)
								, glm::vec4(this->state.upDirection, -upDotPos)
								, glm::vec4(this->state.viewDirection, -viewDotPos)
								, glm::vec4(0.f, 0.f, 0.f, 1.f) );
}

void CameraZ::updatePerspectiveMatrix() {
	if (this->state.perspectiveOn) {
		float farD = this->state.farDist;
		float nearD = this->state.nearDist;
		float top = fabs(nearD) * tanf(this->state.fovVert / 2.0f);
		float bottom = -top;
		//cuidado com possível redundância de levar em conta fovHor separado E ratio
		float right = fabs(nearD) * tanf(this->state.fovHor / 2.0f) * screenRatio;
		//------------------------------------------------------------------------
		float left = -right;

		glm::mat4 Perspec(
			nearD, 0.0f, 0.0f       , 0.0f,
			0.0f, nearD, 0.0f       , 0.0f,
			0.0f, 0.0f, nearD + farD, -farD * nearD,
			0.0f, 0.0f, 1.0f        , 0.0f
		);

		glm::mat4 Ortho(
			2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
			0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
			0.0f, 0.0f, 2.0f / (farD - nearD), -(farD + nearD) / (farD - nearD),
			0.0f, 0.0f, 0.0f, 1.0f
		);

		this->state.perspective = -Ortho * Perspec;
	}

	else {
		float top = this->state.orthoDistance;
		float bottom = -top;
		float right = top * screenRatio;
		float left = -right;
		float farD = this->state.farDist;
		float nearD = this->state.nearDist;

		this->state.perspective = glm::mat4(
			2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
			0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
			0.0f, 0.0f, 2.0f / (farD - nearD), -(farD + nearD) / (farD - nearD),
			0.0f, 0.0f, 0.0f                 ,   1.0f
		);
	}
}

void CameraZ::updateCameraMatrix() {
	this->updateViewMatrix();
	this->updatePerspectiveMatrix();
	this->state.cameraMatrix = this->state.perspective * this->state.view;
	this->cameraProjection = this->state.cameraMatrix;
}