//Para testar renderização com camera e modelo.
//Ideia é passar isso pro viewer, reorganizando pra depender só do RZ_api

#include "render0/model0.hpp"
#include "render0/render0.hpp"
#include "render0/camera.hpp"

#include "RZ_api.hpp"

//esse é o estado vai ficar sendo atualizado e lido pra renderizar - encapsular isso
renderInfo_t renderInfo; 

void controlTest(CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr);

int rz::modelMaterialRenderTest(const char* modelFile) {
    
    //cria recursos necessarios pra renderizar um modelo com controle de camera e manda rodar

	RZ_TRACE("Renderizar modelo lido, com camera e materiais");
	
	static mz::ModelZ testModel(modelFile);
    CameraZ cam = CameraZ::getCameraCenteredOnModel(&testModel, testModel.getCopyBoundingBox(),
        true, true, 60.f, -0.1f, -5000);
        
    renderInfo = setupRenderInfoCameraModelSimple(&cam, &testModel);
    static glm::mat4 modelMatrixMMTest = glm::mat4(1.0f); //centrando pela camera
    
    return renderWithCameraAndModel(&renderInfo, cam, modelMatrixMMTest, SIMPLE_MVP, controlTest);
}

//criar funções e/ou macros pra toggle e testes na input
void controlTest(CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr) {
    
    //sinais de controle definidos em input.hpp e input.cpp
    //testa sinais e faz funcionar input, em especial controle de camera

    float basePosStepSize = 0.025f;
    float constexpr rotStepSize = glm::radians(1.8f);
    float fovStepSize = 0.025f;
    float baseFarStepSize = 0.35f;
    float baseNearStepSize = 0.1f;
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
    //^usadas para evitar de ficar ativando várias vezes enquanto se aperta o botão

    glm::vec3 translationCameraCoordinates(0, 0, 0);
    glm::vec3 rotation(0, 0, 0);

    static float speedMultiplier;
    
    float posStepSize = basePosStepSize;
    float nearStepSize = baseNearStepSize;
    float farStepSize = baseFarStepSize;

    if (camera_ptr->lookAtIsOn() && useCameraCoordinates) {
        speedMultiplier = glm::length(camera_ptr->getPosition() - camera_ptr->getLookAtPos());
        if (speedMultiplier < 0.25f) speedMultiplier = 0.25f;
    }
    posStepSize *= speedMultiplier;

    float nearFarMultiplier = fabs(camera_ptr->getFarDist() - camera_ptr->getNearDist())/100.f;
    nearStepSize *= nearFarMultiplier;
    farStepSize *= nearFarMultiplier;

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
        renderInfo_ptr->colorForUniform.r += colorStep;
        if (renderInfo_ptr->colorForUniform.r > 1)
            renderInfo_ptr->colorForUniform.r -= 1;
        /*
        renderInfo_ptr->clearColor[0] += colorStep;
        if (renderInfo_ptr->clearColor[0] > 1)
            renderInfo_ptr->clearColor[0] -= 1;
        */
    }
    if (keyboardInput.decreaseR) {
        renderInfo_ptr->colorForUniform.r -= colorStep;
        if (renderInfo_ptr->colorForUniform.r < 0)
            renderInfo_ptr->colorForUniform.r += 1;
        /*
        renderInfo_ptr->clearColor[0] -= colorStep;
        if (renderInfo_ptr->clearColor[0] < 0)
            renderInfo_ptr->clearColor[0] += 1;
        */
    }

    if (keyboardInput.increaseG) {
        renderInfo_ptr->colorForUniform.g += colorStep;
        if (renderInfo_ptr->colorForUniform.g > 1)
            renderInfo_ptr->colorForUniform.g -= 1;
        /* 
        renderInfo_ptr->clearColor[1] += colorStep;
        if (renderInfo_ptr->clearColor[1] > 1)
            renderInfo_ptr->clearColor[1] -= 1;
        */
    }
    if (keyboardInput.decreaseG) {
        renderInfo_ptr->colorForUniform.g -= colorStep;
        if (renderInfo_ptr->colorForUniform.g < 0)
            renderInfo_ptr->colorForUniform.g += 1;
        /*
        renderInfo_ptr->clearColor[1] -= colorStep;
        if (renderInfo_ptr->clearColor[1] < 0)
            renderInfo_ptr->clearColor[1] += 1;
        */
    }

    if (keyboardInput.increaseB) {
        renderInfo_ptr->colorForUniform.b += colorStep;
        if (renderInfo_ptr->colorForUniform.b > 1)
            renderInfo_ptr->colorForUniform.b -= 1;
        /*
        renderInfo_ptr->clearColor[2] += colorStep;
        if (renderInfo_ptr->clearColor[2] > 1)
            renderInfo_ptr->clearColor[2] -= 1;
        */
    }
    if (keyboardInput.decreaseB) {
        renderInfo_ptr->colorForUniform.b -= colorStep;
        if (renderInfo_ptr->colorForUniform.b < 0)
            renderInfo_ptr->colorForUniform.b += 1;
        /*
        renderInfo_ptr->clearColor[2] -= colorStep;
        if (renderInfo_ptr->clearColor[2] < 0)
            renderInfo_ptr->clearColor[2] += 1;
        */
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
    }

}
