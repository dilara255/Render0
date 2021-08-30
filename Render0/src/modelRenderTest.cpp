#include "render0/model0.hpp"
#include "render0/render0.hpp"
#include "render0/camera.hpp"

#include "RZ_api.hpp"

glm::mat4 getCenteringModelMatrix(mz::ModelZ modelToCenter);
CameraZ getTestCamera(mz::ModelZ* model_ptr);
renderInfo_t setupRenderInfo(CameraZ* camera_ptr, mz::ModelZ* model_ptr);
void controlTest(CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr);

renderInfo_t renderInfo;

int rz::modelMaterialRenderTest(const char* modelFile) {

	RZ_TRACE("Renderizar modelo lido, com camera e materiais");
	
	static mz::ModelZ testModel("models/2sap.max");
    CameraZ camera = getTestCamera(&testModel);
    renderInfo = setupRenderInfo(&camera, &testModel);
    static glm::mat4 modelMatrixMMTest = getCenteringModelMatrix(testModel);
    
    return renderWithCameraAndModel(&renderInfo, camera, modelMatrixMMTest, SIMPLE_MVP, controlTest);
}


glm::mat4 getCenteringModelMatrix(mz::ModelZ modelToCenter) {
    //RETORNANDO IDENTIDADE PQ TO CENTRANDO PELA CAMERA

    /*
    float cx = modelToCenter.getBoundingBoxCenter().x;
    float cy = modelToCenter.getBoundingBoxCenter().y;
    float cz = modelToCenter.getBoundingBoxCenter().z;
    float cw = modelToCenter.getBoundingBoxCenter().w;

    printf("\n\nCenter: %f, %f, %f, %f\n\n", cx,cy,cz,cw);
    */

    glm::mat4 modelMatrix = {
        glm::vec4( 1.f, 0  , 0  , 0   ),
        glm::vec4( 0  , 1.f, 0  , 0   ),
        glm::vec4( 0  , 0  , 1.f, 0   ),
        glm::vec4( 0  , 0  , 0  , 1.f ),
    };

    return modelMatrix;
}

CameraZ getTestCamera(mz::ModelZ* model_ptr) {

    RZ_TRACE("Inicializando camera...");

    cameraState_t initialState;

    initialState.lookAtActive = true;
    initialState.perspectiveOn = true;

    float bBoxDiagonal = model_ptr->getBoundingBoxDiagonal();
    float cx = model_ptr->getBoundingBoxCenter().x;
    float cy = model_ptr->getBoundingBoxCenter().y;
    float cz = model_ptr->getBoundingBoxCenter().z;

    initialState.nearDist = -0.1f;
    initialState.farDist = -100;
    initialState.fovHor = glm::radians(60.f);
    initialState.fovVert = glm::radians(60.f);
    initialState.orthoDistance = bBoxDiagonal / 2.f;

    float distanceToFitModel = bBoxDiagonal / (2*tanf(initialState.fovVert / 2.f));

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

renderInfo_t setupRenderInfo(CameraZ* camera_ptr, mz::ModelZ* model_ptr) {
    
    RZ_TRACE("Renderizar modelo lido de arquivo com material...");

    GLuint  VAOs[NumVAOs];
    GLuint  Buffers[NumBuffersWithModels];

    int numTriangles = model_ptr->getNumberTriangles();
    int numMaterials = model_ptr->getNumberMaterials();

    GLuint  numVertices = 3 * numTriangles;

    RZ_TRACE("Gerando vertex array e buffers...");

    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    glGenBuffers(NumBuffersWithModels, Buffers);

    RZ_TRACE("Preparando Buffer Posicao...");   

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec4)
                , model_ptr->posBuffer
                , GL_STATIC_DRAW);
    
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
    
    RZ_TRACE("Preparando Buffer Cor Difusa...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[DiffuseBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4)
        , model_ptr->diffuseBuffer
        , GL_STATIC_DRAW);

    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vColor);

    RZ_TRACE("Preparando Buffer Normal...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[NormBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4)
        , model_ptr->normBuffer
        , GL_STATIC_DRAW);

    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vNormal);

    RZ_TRACE("Preparando Buffer Ambiente...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[AmbientBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4)
        , model_ptr->ambientBuffer
        , GL_STATIC_DRAW);

    glVertexAttribPointer(vAmbient, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vAmbient);

    RZ_TRACE("Preparando Buffer Especular...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[SpecularBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4)
        , model_ptr->specularBuffer
        , GL_STATIC_DRAW);
    
    glVertexAttribPointer(vSpecular, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vSpecular);

    RZ_TRACE("Preparando Buffer Coef Brilho Especular...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[SpcCoefBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4)
        , model_ptr->spcShineCoefBuffer
        , GL_STATIC_DRAW);

    glVertexAttribPointer(vSpcCoef, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vSpcCoef);

    RZ_TRACE("Pronto. Definindo cor de fundo/clear e juntando as infos pra renderizar...");

    const rz::colorf_t black = { 0.0f, 0.0f, 0.0f, 0.0f };

    renderInfo_t renderInfo = {
        black[0],
        black[1],
        black[2],
        black[3],
        VAOs[Triangles],
        numVertices,
        renderWindow.window,
        0,
        camera_ptr,
        model_ptr
    };

    return renderInfo;
}

void controlTest(CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr) {

    float posStepSize = 0.025f;
    float constexpr rotStepSize = glm::radians(1.8f);
    float fovStepSize = 0.025f;
    float farStepSize = 0.35f;
    float nearStepSize = 0.1f;
    float colorStep = 0.01f;

    bool hasToUpdate = false;

    static bool useCameraCoordinates = true;

    static bool blockToogleUseCameraCoordinates = false;
    static bool blockTooglePerspective = false;
    static bool blockToogleLookAt = false;
    static bool blockReset = false;
    static bool blockCycleDrawMode = false;
    static bool blockToogleCulling = false;
    static bool blockToogleCCWtoCW = false;

    glm::vec3 translationCameraCoordinates(0, 0, 0);
    glm::vec3 rotation(0, 0, 0);

    if (camera_ptr->lookAtIsOn() && useCameraCoordinates) {
        float speedMultiplier = glm::length(camera_ptr->getPosition() - camera_ptr->getView());
        if (speedMultiplier < 0.25f) speedMultiplier = 0.25f;
        else if (speedMultiplier > 50.f) speedMultiplier = 50.f;
        posStepSize *= speedMultiplier;
    }

    if (keyboardInput.closeWindowPressed) {
        glfwSetWindowShouldClose(renderInfo_ptr->window, GL_TRUE);
    }

    if (keyboardInput.toggleCullingPressed) {
        if (!blockToogleCulling) {
            renderInfo_ptr->shouldCull = !renderInfo_ptr->shouldCull;

            blockToogleCulling = true;

            if (renderInfo_ptr->shouldCull) RZ_TRACE("Culling ativado");
            else RZ_TRACE("Culling desativado");
        }
    }
    else blockToogleCulling = false;

    if (keyboardInput.toggleCCWtoCWPressed) {
        if (!blockToogleCCWtoCW) {
            if (renderInfo_ptr->faceDirectionForCulling == GL_CCW) {
                renderInfo_ptr->faceDirectionForCulling = GL_CW;
                RZ_TRACE("Culling com Front Face CW");
            }
            else {
                renderInfo_ptr->faceDirectionForCulling = GL_CCW;
                RZ_TRACE("Culling com Front Face CCW");
            }

            blockToogleCCWtoCW = true;
        }
    }
    else blockToogleCCWtoCW = false;

    if (keyboardInput.rightPressed) {
        translationCameraCoordinates.x += posStepSize;
    }
    if (keyboardInput.leftPressed) {
        translationCameraCoordinates.x -= posStepSize;
    }

    if (keyboardInput.upPressed) {
        translationCameraCoordinates.y += posStepSize;
    }
    if (keyboardInput.downPressed) {
        translationCameraCoordinates.y -= posStepSize;
    }

    if (keyboardInput.forwardPressed) {
        translationCameraCoordinates.z -= posStepSize;
    }

    if (keyboardInput.backwardPressed) {
        translationCameraCoordinates.z += posStepSize;
    }

    hasToUpdate = ((translationCameraCoordinates.x != 0)
        || (translationCameraCoordinates.y != 0)
        || (translationCameraCoordinates.z != 0));

    if (keyboardInput.yawLeftPressed) {
        rotation.y -= rotStepSize;
    }
    if (keyboardInput.yawRightPressed) {
        rotation.y += rotStepSize;
    }

    if (keyboardInput.pitchUpPressed) {
        rotation.x += rotStepSize;
    }
    if (keyboardInput.pitchDownPressed) {
        rotation.x -= rotStepSize;
    }

    if (keyboardInput.rollCWPressed) {
        rotation.z += rotStepSize;
    }
    if (keyboardInput.rollCCWPressed) {
        rotation.z -= rotStepSize;
    }
    
    hasToUpdate = hasToUpdate || ((rotation.x != 0) || (rotation.y != 0) || (rotation.z != 0));

    if (keyboardInput.increaseFOVPressed) {
        camera_ptr->setFovVer(camera_ptr->getFovVer() + fovStepSize);
        camera_ptr->setFovHor(camera_ptr->getFovHor() + fovStepSize);

        hasToUpdate = true;
    }
    if (keyboardInput.decreaseFOVPressed) {
        camera_ptr->setFovVer(camera_ptr->getFovVer() - fovStepSize);
        camera_ptr->setFovHor(camera_ptr->getFovHor() - fovStepSize);

        hasToUpdate = true;
    }

    if (keyboardInput.increaseFar) {
        camera_ptr->changeFarDistBy(farStepSize);

        hasToUpdate = true;
    }
    if (keyboardInput.decreaseFar) {
        camera_ptr->changeFarDistBy(-farStepSize);

        hasToUpdate = true;
    }

    if (keyboardInput.increaseNear) {
        camera_ptr->changeNearDistBy(nearStepSize);

        hasToUpdate = true;
    }
    if (keyboardInput.decreaseNear) {
        camera_ptr->changeNearDistBy(-nearStepSize);

        hasToUpdate = true;
    }

    if (keyboardInput.increaseR) {
        renderInfo_ptr->clearColor[0] += colorStep;
        if (renderInfo_ptr->clearColor[0] > 1)
            renderInfo_ptr->clearColor[0] -= 1;
    }
    if (keyboardInput.decreaseR) {
        renderInfo_ptr->clearColor[0] -= colorStep;
        if (renderInfo_ptr->clearColor[0] < 0)
            renderInfo_ptr->clearColor[0] += 1;
    }

    if (keyboardInput.increaseG) {
        renderInfo_ptr->clearColor[1] += colorStep;
        if (renderInfo_ptr->clearColor[1] > 1)
            renderInfo_ptr->clearColor[1] -= 1;
    }
    if (keyboardInput.decreaseG) {
        renderInfo_ptr->clearColor[1] -= colorStep;
        if (renderInfo_ptr->clearColor[1] < 0)
            renderInfo_ptr->clearColor[1] += 1;
    }

    if (keyboardInput.increaseB) {
        renderInfo_ptr->clearColor[2] += colorStep;
        if (renderInfo_ptr->clearColor[2] > 1)
            renderInfo_ptr->clearColor[2] -= 1;
    }
    if (keyboardInput.decreaseB) {
        renderInfo_ptr->clearColor[2] -= colorStep;
        if (renderInfo_ptr->clearColor[2] < 0)
            renderInfo_ptr->clearColor[2] += 1;
    }

    if (keyboardInput.cycleDrawModePressed) {
        if (!blockCycleDrawMode) {
            renderInfo_ptr->mode++;
            if(renderInfo_ptr->mode > 2) renderInfo_ptr->mode -= 3;

            blockCycleDrawMode = true;

            if (renderInfo_ptr->mode == 0) RZ_TRACE("Usando modo Triangulos");
            else if (renderInfo_ptr->mode == 1) RZ_TRACE("Usando modo Linhas");
            else RZ_TRACE("Usando modo Pontos");
        }
    }
    else blockCycleDrawMode = false;
    
    if (keyboardInput.toggleMoveCameraOrWorldCoords) {
        if (!blockToogleUseCameraCoordinates) {
            useCameraCoordinates = !useCameraCoordinates;

            blockToogleUseCameraCoordinates = true;

            if (useCameraCoordinates) RZ_TRACE("Usando Coordenadas de Camera");
            else RZ_TRACE("Usando Coordenadas de Mundo");
        }
    }
    else blockToogleUseCameraCoordinates = false;

    if (keyboardInput.toggleLookatPressed) {
        if (!blockToogleLookAt) {
            camera_ptr->lookAtToggle();

            blockToogleLookAt = true;
            hasToUpdate = true;

            bool lookatIsOn = camera_ptr->lookAtIsOn();
            if (lookatIsOn) RZ_TRACE("Lookat Ativado");
            else RZ_TRACE("Lookat Desativado");
        }
    }
    else blockToogleLookAt = false;

    if (keyboardInput.toggleOrthoPerspectivePressed) {
        if (!blockTooglePerspective) {
            camera_ptr->perspectiveToggle();

            blockTooglePerspective = true;
            hasToUpdate = true;

            bool perspectiveIsOn = camera_ptr->perspectiveIsOn();
            if (perspectiveIsOn) RZ_TRACE("Proj. Perspectiva Ativada");
            else RZ_TRACE("Proj. Orthografica Ativado");
        }
    }
    else blockTooglePerspective = false;

    if (keyboardInput.resetViewPressed) {
        if (!blockReset) {
            camera_ptr->resetCamera();

            blockReset = true;
            hasToUpdate = true;

            RZ_TRACE("Camera Resetada");
        }
    }
    else blockReset = false;

    if (hasToUpdate) {

        if (useCameraCoordinates) {
            camera_ptr->translateCameraCoord(translationCameraCoordinates);
        }
        else camera_ptr->translateWorldCoord(translationCameraCoordinates);

        if ( ! camera_ptr->lookAtIsOn() ) {
            
            if (rotation.y != 0) {
                camera_ptr->yaw(rotation.y);
            }
            if (rotation.x != 0) {
                camera_ptr->pitch(rotation.x);
            }
            if (rotation.z != 0) {
                camera_ptr->roll(rotation.z);
            }
        }

        camera_ptr->updateCameraMatrix();

        /*
        printf("\tcamera position: \t%f, %f, %f\n", camera->getPosition().x
              , camera->getPosition().y, camera->getPosition().z);

        printf("\tview direction: \t%f, %f, %f, fovV: %f\n", camera->getView().x
            , camera->getView().y, camera->getView().z, camera->getFovVer());
        */
    }

}
