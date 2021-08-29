#include "render0/model0.hpp"
#include "render0/render0.hpp"
#include "render0/camera.hpp"

#include "RZ_api.hpp"

CameraZ getTestCamera(float bBoxDiagonal);
renderInfo_t setupRenderInfo(int shader, CameraZ* camera_ptr, mz::ModelZ* model_ptr);
void controlTest(CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr);

renderInfo_t renderInfo;
glm::mat4 modelMatrix;

int initModelRenderTest() {

	RZ_TRACE("Renderizar modelo lido, com camera");
	
	static mz::ModelZ testModel("models/Teste3DMax.max");
    CameraZ camera = getTestCamera(testModel.getBoundingBoxDiagonal());
    renderInfo = setupRenderInfo(SIMPLE_MVP, &camera, &testModel);
    
    float cx = testModel.getBoundingBoxCenter().x;
    float cy = testModel.getBoundingBoxCenter().y;
    float cz = testModel.getBoundingBoxCenter().z;
    float cw = testModel.getBoundingBoxCenter().w;

    modelMatrix = {
        glm::vec4(1.f, 0  , 0  , -cx),
        glm::vec4(0  , 1.f, 0  , -cy),
        glm::vec4(0  , 0  , 1.f, -cz),
        glm::vec4(0  , 0  , 0  , -cw),
    };

    return renderWithCameraAndModel(&renderInfo, camera, &modelMatrix, controlTest);
}

CameraZ getTestCamera(float bBoxDiagonal) {

    RZ_TRACE("Inicializando camera...");

    cameraState_t initialState;

    initialState.lookAtActive = true;
    initialState.perspectiveOn = true;

    initialState.nearDist = -0.1f;
    initialState.farDist = -100;
    initialState.fovHor = glm::radians(60.f);
    initialState.fovVert = glm::radians(60.f);
    initialState.orthoDistance = bBoxDiagonal / 2.f;

    float distanceToFitModel = tanf(initialState.fovVert / 2.f) * (bBoxDiagonal / 2.f);

    initialState.position = glm::vec3(0.0f, 0.0f, distanceToFitModel);

    initialState.lookatPosition = glm::vec3(0, 0, 0);
    initialState.viewDirection = glm::vec3(0.0f, 0.0f, -1);
    initialState.upDirection = glm::vec3(0, 1, 0);
    initialState.sideDirection = glm::vec3(1, 0, 0);

    CameraZ camera(initialState);

    camera.updateCameraMatrix();

    RZ_INFO("Camera iniciada!");

    return camera;
}

renderInfo_t setupRenderInfo(int shader, CameraZ* camera_ptr, mz::ModelZ* model_ptr) {
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
        GL_TRIANGLE_STRIP, //GL_LINE_LOOP GL_POINTS
        camera_ptr,
        model_ptr
    };

    return renderInfo;
}

void controlTest(CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr) {

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

    hasToUpdate = ((translationCameraCoordinates.x != 0)
        || (translationCameraCoordinates.y != 0)
        || (translationCameraCoordinates.z != 0));

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

        camera_ptr->updateCameraMatrix();

        /*
        printf("\tcamera position: \t%f, %f, %f\n", camera->getPosition().x
              , camera->getPosition().y, camera->getPosition().z);

        printf("\tview direction: \t%f, %f, %f, fovV: %f\n", camera->getView().x
            , camera->getView().y, camera->getView().z, camera->getFovVer());
        */
    }

}
