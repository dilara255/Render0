//Dois testes iniciais, para desenhar triângulos definidos em código, com e sem câmera
//Talvez criar uma pasta pra testes? Eu acho que isso aqui eu não vou manter na verdade,
//mas fica por enquanto

#include "miscStdHeaders.h"

#include "render0/render0.hpp"
#include "render0/model0.hpp"
#include "RZ_API/RZ_api.hpp"
#include "logAPI.hpp"

CameraZ getTestCamera();
renderInfo_t setupTestTriangleToRender(int shader);
void controlTestCamera(CameraZ* camera);

renderInfo_t testRenderInfo;

namespace rz {

    int testTriangle(void) {
        return renderSimple(setupTestTriangleToRender(SIMPLE));
    }

    int testTriangleWithCamera(void) {
        RZ_TRACE("Desenhar triangulinho colorido de teste COM CAMERA...");
        CameraZ camera = getTestCamera();
        camera.updateCameraMatrix();
        testRenderInfo = setupTestTriangleToRender(SIMPLE_CAMERA);
        return renderWithCamera(&testRenderInfo, camera, controlTestCamera);
    }
}

renderInfo_t setupTestTriangleToRender(int shader)
{
    GLuint  VAOs[NumVAOs];
    GLuint  Buffers[NumBuffers];

    RZ_TRACE("Desenhar triangulinho colorido de teste...");

    const GLuint  NumVertices = 3;

    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);

    //criei typedef render.hpp
    struct VertexData {
        GLubyte color[4];
        GLfloat position[4];
    };

    VertexData vertices[NumVertices] = {
        {{ 255,   0,   0, 255 }, { -0.90f, -0.90f, 0, 1 }},
        {{   0, 255,   0, 255 }, {  0.90f, -0.90f, 0, 1 }},
        {{   0,   0, 255, 255 }, { -0.90f,  0.90f, 0, 1 }},
    };


    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    RZ_TRACE("Buffers preparados...");

    //shaders ---v

    RZ_TRACE("Carregar shaders...");

    shaderPrograms[shader].shaderProgramId = LoadShaders(shaderPrograms[shader].shaderInfo);
    RZ_TRACE("Ligar programa...");
    glUseProgram(shaderPrograms[shader].shaderProgramId);

    RZ_TRACE("Shaders ligados...");

    glVertexAttribPointer(vPosition, 2, GL_FLOAT
        , GL_FALSE, sizeof(VertexData)
        , BUFFER_OFFSET(sizeof(vertices[0].color)));
    glVertexAttribPointer(vColor, 4, GL_UNSIGNED_BYTE
        , GL_TRUE, sizeof(VertexData)
        , BUFFER_OFFSET(0));

    //shaders ---^

    RZ_TRACE("Vai renderizar...");

    const rz::colorf_t black = { 0.0f, 0.0f, 0.0f, 0.0f };

    renderInfo_t renderInfo = {
        black[0],
        black[1],
        black[2],
        black[3],
        VAOs[Triangles],
        NumVertices,
        renderWindow.window,
        GL_TRIANGLE_STRIP
        //GL_LINE_LOOP GL_POINTS
    };

    return renderInfo;
}

CameraZ getTestCamera() {
    
    RZ_TRACE("Inicializando camera...");

    cameraState_t initialState;

    initialState.lookAtActive = true;
    initialState.perspectiveOn = true;

    initialState.position = glm::vec3(0.0f,0.0f,2.f);

    initialState.lookatPosition = glm::vec3(0, 0, 0);
    initialState.viewDirection = glm::vec3(0.0f, 0.0f, -1);
    initialState.upDirection = glm::vec3(0, 1, 0);
    initialState.sideDirection = glm::vec3(1, 0, 0);

    initialState.nearDist = -0.1f;
    initialState.farDist = -100;
    initialState.fovHor = glm::radians(60.f);
    initialState.fovVert = glm::radians(60.f);
    initialState.orthoDistance = fabs(sinf(initialState.fovHor/2)* initialState.farDist/2.f);

    CameraZ camera(initialState);

    RZ_INFO("Camera iniciada!");

    return camera;
}

void controlTestCamera(CameraZ* camera) {

    static bool useCameraCoordinates = true;

    static bool blockToogleUseCameraCoordinates = false;
    static bool blockTooglePerspective = false;
    static bool blockToogleLookAt = false;
    static bool blockReset = false;

    bool hasToUpdate = false;
    
    glm::vec3 translationCameraCoordinates(0, 0, 0);
    float posStepSize = 0.025f;
    float fovStepSize = 0.025f;
    float farStepSize = 1.f;
    float nearStepSize = 0.1f;
    float colorStep = 0.01f;

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

    hasToUpdate = (  (translationCameraCoordinates.x != 0) 
                  || (translationCameraCoordinates.y != 0)
                  || (translationCameraCoordinates.z != 0) );

    if (keyboardInput.increaseFOVPressed) {
        camera->setFovVer(camera->getFovVer() + fovStepSize);
        camera->setFovHor(camera->getFovHor() + fovStepSize);
        
        hasToUpdate = true;
    }
    if (keyboardInput.decreaseFOVPressed) {
        camera->setFovVer(camera->getFovVer() - fovStepSize);
        camera->setFovHor(camera->getFovHor() - fovStepSize);

        hasToUpdate = true;
    }

    if (keyboardInput.increaseFar) {
        camera->changeFarDistBy(farStepSize);

        hasToUpdate = true;
    }
    if (keyboardInput.decreaseFar) {
        camera->changeFarDistBy(-farStepSize);

        hasToUpdate = true;
    }

    if (keyboardInput.increaseNear) {
        camera->changeNearDistBy(nearStepSize);

        hasToUpdate = true;
    }
    if (keyboardInput.decreaseNear) {
        camera->changeNearDistBy(-nearStepSize);

        hasToUpdate = true;
    }

    if (keyboardInput.increaseR) {
        testRenderInfo.clearColor[0] += colorStep;
        if (testRenderInfo.clearColor[0] > 1)
            testRenderInfo.clearColor[0] -= 1;
    }
    if (keyboardInput.decreaseR) {
        testRenderInfo.clearColor[0] -= colorStep;
        if (testRenderInfo.clearColor[0] < 0)
            testRenderInfo.clearColor[0] += 1;
    }

    if (keyboardInput.increaseG) {
        testRenderInfo.clearColor[1] += colorStep;
        if (testRenderInfo.clearColor[1] > 1)
            testRenderInfo.clearColor[1] -= 1;
    }
    if (keyboardInput.decreaseG) {
        testRenderInfo.clearColor[1] -= colorStep;
        if (testRenderInfo.clearColor[1] < 0)
            testRenderInfo.clearColor[1] += 1;
    }

    if (keyboardInput.increaseB) {
        testRenderInfo.clearColor[2] += colorStep;
        if (testRenderInfo.clearColor[2] > 1)
            testRenderInfo.clearColor[2] -= 1;
    }
    if (keyboardInput.decreaseB) {
        testRenderInfo.clearColor[2] -= colorStep;
        if (testRenderInfo.clearColor[2] < 0)
            testRenderInfo.clearColor[2] += 1;
    }
    
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
            camera->lookAtToggle();
            
            blockToogleLookAt = true;
            hasToUpdate = true;
            
            bool lookatIsOn = camera->lookAtIsOn();
            if (lookatIsOn) RZ_TRACE("Lookat Ativado");
            else RZ_TRACE("Lookat Desativado");
        }
    }
    else blockToogleLookAt = false;

    if (keyboardInput.toggleOrthoPerspectivePressed) {
        if (!blockTooglePerspective) {
            camera->perspectiveToggle();
            
            blockTooglePerspective = true;
            hasToUpdate = true;

            bool perspectiveIsOn = camera->perspectiveIsOn();
            if (perspectiveIsOn) RZ_TRACE("Proj. Perspectiva Ativada");
            else RZ_TRACE("Proj. Orthografica Ativado");
        }
    } 
    else blockTooglePerspective = false;

    if (keyboardInput.resetViewPressed) {
        if (!blockReset) {
            camera->resetCamera();

            blockReset = true;
            hasToUpdate = true;

            RZ_TRACE("Camera Resetada");
        }
    }
    else blockReset = false;

    if (hasToUpdate) {

        if (useCameraCoordinates) {
            camera->translateCameraCoord(translationCameraCoordinates);
        }
        else camera->translateWorldCoord(translationCameraCoordinates);

        camera->updateCameraMatrix();

        /*
        printf("\tcamera position: \t%f, %f, %f\n", camera->getPosition().x
              , camera->getPosition().y, camera->getPosition().z);

        printf("\tview direction: \t%f, %f, %f, fovV: %f\n", camera->getView().x
            , camera->getView().y, camera->getView().z, camera->getFovVer());
        */
    }
}