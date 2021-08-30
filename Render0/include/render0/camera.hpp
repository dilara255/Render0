#pragma once

#include "miscStdHeaders.h"

#include "logAPI.hpp"

#define DEFAULT_WIDTH (640)
#define DEFAULT_HEIGHT (480)

inline glm::float32 screenRatio;

//supposes normalized view, up and side directions, and keeps them that way
typedef struct cameraState_st {
	bool lookAtActive;
	bool perspectiveOn;

	glm::vec3 position;
	
	glm::vec3 lookatPosition;
	glm::vec3 viewDirection;
	glm::vec3 upDirection;
	glm::vec3 sideDirection;
	
	glm::float32 orthoDistance;
	glm::float32 nearDist;
	glm::float32 farDist;
	radians fovHor;
	radians fovVert;

	glm::mat4 cameraMatrix;
	glm::mat4 view;
	glm::mat4 perspective;
}cameraState_t;

class CameraZ {

public:

	CameraZ(cameraState_t initialState);

	void translateWorldCoord(glm::vec3 translation);
	void translateCameraCoord(glm::vec3 translation);
	glm::vec3 CameraZ::getPosition();

	void roll(radians angle);
	void yaw(radians angle);
	void pitch(radians angle);
	glm::vec3 CameraZ::getView();

	void lookAtToggle();
	void lookAtTurnOn();
	void lookAtTurnOff();
	bool lookAtIsOn();
	void lookAtSetPos(glm::vec3 position);

	void perspectiveToggle();
	void perspectiveTurnOn();
	void perspectiveTurnOff();
	bool perspectiveIsOn();

	void updateCameraMatrix();
	glm::mat4 getCameraMatrix();

	radians getNearDist();
	radians getFarDist();
	void changeNearDistBy(glm::float32 nearDistChange);
	void changeFarDistBy(glm::float32 farDistChange);

	radians getFovHor();
	radians getFovVer();
	void setFovHor(radians fovHor);
	void setFovVer(radians fovVert);

	void resetCamera();

	glm::mat4 cameraProjection;

private:

	void rotate(radians angle, glm::vec3* directionFrom, glm::vec3* directionTo);

	void updateViewToLookat();
	void updateViewMatrix();
	void updatePerspectiveMatrix();

	cameraState_t state;
	cameraState_t initialState;
};

