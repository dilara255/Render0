#pragma once

//Defini��es, estruturas e fun��es para renderiza��o vis�veis a outros subsistemas.
//(bastante acoplamente atrav�s daqui)
//Fun��es de inicializa��o de shaders e de renderiza��o.

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "RZ_API/RZ_api.hpp"

#include "logAPI.hpp"

#include "camera.hpp"
#include "input0.hpp"
#include "model0.hpp"
  
#define BUFFER_OFFSET(a) ((void*)(a))
#define NUMBER_SHADERS 3

enum VAO_IDs { Triangles, NumVAOs };

enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum BufferWithModels_IDs { PosBuffer, DiffuseBuffer, NormBuffer
                          , AmbientBuffer, SpecularBuffer
                          , SpcCoefBuffer, NumBuffersWithModels };

enum Shader_Types { VERT, FRAG, NOT_USING };
enum Shaders { SIMPLE, SIMPLE_CAMERA, SIMPLE_MVP };

enum Attrib_IDs { vPosition = 0, vColor = 1 , vNormal = 2, vAmbient = 3
                , vSpecular = 4, vSpcCoef = 5};

enum Uniform_IDs {vPV_matrix, vModel_matrix, vColorUniform, NUMBER_UNIFORMS};

typedef struct windowParams_st {
    GLFWwindow* window;
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    const char* title = "Viewer0 " SYSTEM_NAME "-" PLATFORM_NAME " " CONFIG_NAME;
    int guiWidth = DEFAULT_GUI_WIDTH;
} windowParams_t;

typedef struct renderInfo_st {
    rz::colorf_t clearColor;
    GLuint VAO;
    GLuint numVertices;
    GLFWwindow* window;
    int mode;
    CameraZ* cameraState_ptr;
    mz::ModelZ* model_ptr = (mz::ModelZ * )NULL;
    int modes[3] = { GL_TRIANGLES , GL_LINE_LOOP , GL_POINTS };
    int faceDirectionForCulling = GL_CW; //GL_CCW
    bool shouldCull = true;
    glm::vec4 colorForUniform = glm::vec4(0.55f, 0.1f, 0.85f, 1.f);
    GLint uniformLocations[NUMBER_UNIFORMS];
    windowParams_t windowParameters;
    //provavelmente seja melhor criar um estado de render eventualmente
    int swapInterval = 0;
    float frameTimeMS = 1;
    int shadingToUse = SIMPLE_MVP;
} renderInfo_t;

typedef struct vertexDataCP_st {
    GLubyte color[4];
    GLfloat position[4];
} vertexDataCP_t;

typedef struct vertexDataPMM_st {
    GLubyte ambientColor[4];
    GLubyte diffuseColor[4];
    GLubyte specularColor[4];

    GLfloat position[4];
    GLfloat normal[4];

    GLfloat specShineCoef;
} vertexDataPMM_t;

typedef struct {
    GLenum       type;
    const char* filename;
    GLuint       shader;
} shaderInfo_t;

typedef struct shaderPrograms_st {
    int shadersID;
    shaderInfo_t shaderInfo[NOT_USING + 1];
    GLuint shaderProgramId;
}shaderPrograms_t;

inline shaderPrograms_t shaderPrograms[NUMBER_SHADERS];
inline windowParams_t renderWindow;

GLuint LoadShaders(shaderInfo_t* shaders);

void initShaderPrograms();
renderInfo_t setupRenderInfoCameraModelSimple(CameraZ* camera_ptr, mz::ModelZ* model_ptr);

void setupRender(renderInfo_t* renderInfo);
bool renderOgl(renderInfo_t* renderInfo_ptr, CameraZ* camera_ptr, float* modelMatrixStart_ptr,
            void (*controlTest) (CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr));

void mainLoop(renderInfo_t* renderInfo_ptr, CameraZ* camera_ptr,
    float* modelMatrixStart_ptr,
    void (*controlTest) (CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr));

void checkOGlErrors();

///////////////////////////////////////////////
///////////////////////////////////////////////
//Other Rendering modes, to be cleaned up soon:
///////////////////////////////////////////////

int renderSimple(renderInfo_t renderInfo);
int renderWithCamera(renderInfo_t* renderInfo, CameraZ camera
    , void (*controlTestCamera) (CameraZ* camera));
int renderWithCameraAndModel(renderInfo_t* renderInfo, CameraZ camera, glm::mat4 modelMatrix
    , int shader, void (*controlTest) (CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr));