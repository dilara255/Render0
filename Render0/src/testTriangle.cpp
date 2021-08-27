#include "miscStdHeaders.h"

#include "render0/render0.hpp"
#include "RZ_API/RZ_api.hpp"
#include "logAPI.hpp"

CameraZ getTestCamera();
renderInfo_t setupTestTriangleToRender(int shader);

namespace rz {

    int testTriangle(void) {
        return renderSimple(setupTestTriangleToRender(SIMPLE));
    }

    int testTriangleWithCamera(void) {
        RZ_TRACE("Desenhar triangulinho colorido de teste COM CAMERA...");
        CameraZ camera = getTestCamera();
        camera.updateCameraMatrix();
        return renderWithCamera(setupTestTriangleToRender(SIMPLE_CAMERA), camera);
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
        {{ 255,   0,   0, 255 }, { -0.90f, -0.90f }},
        {{   0, 255,   0, 255 }, {  0.85f, -0.90f }},
        {{   0,   0, 255, 255 }, { -0.90f,  0.85f }},
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

    initialState.position = glm::vec3(-0.5f,0.5f,10);

    initialState.lookatPosition = glm::vec3(0, 0, 0);
    initialState.viewDirection = glm::vec3(0.1f, -0.1f, -1);
    initialState.upDirection = glm::vec3(0, 1, 0);
    initialState.sideDirection = glm::vec3(1, 0, 0);

    initialState.orthoDistance = 10;
    initialState.nearDist = 0.1f;
    initialState.farDist = 100;
    initialState.fovHor = radians(60);
    initialState.fovVert = radians(60);

    CameraZ camera(initialState);

    RZ_INFO("Camera iniciada!");

    return camera;
}