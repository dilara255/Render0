#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "RZ_API/RZ_api.hpp"
#include "logAPI.hpp"

#include "camera.hpp"

//#ifndef RENDER_GLOBALS
    
    #define BUFFER_OFFSET(a) ((void*)(a))
    #define NUMBER_SHADERS 2

    enum VAO_IDs { Triangles, NumVAOs };
    enum Buffer_IDs { ArrayBuffer, NumBuffers };
    enum Shader_Types { VERT, FRAG, NOT_USING };
    enum Shaders { SIMPLE, SIMPLE_CAMERA };
    enum Attrib_IDs { vPosition = 0, vColor = 1 };

    typedef struct windowParams_st {
        GLFWwindow* window;
        int width = DEFAULT_WIDTH;
        int height = DEFAULT_HEIGHT;
        const char* title = "Viewer0 " SYSTEM_NAME "-" PLATFORM_NAME " " CONFIG_NAME;
    } windowParams_t;

    typedef struct renderInfo_st {
        rz::colorf_t clearColor;
        GLuint VAO;
        GLuint numVertices;
        GLFWwindow* window;
        GLenum mode;
        cameraState_t* cameraState_ptr;
    } renderInfo_t;

    typedef struct vertexDataCP_st {
        GLubyte color[4];
        GLfloat position[4];
    } vertexDataCP_t;

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
//    #define RENDER_GLOBALS
//#endif

GLuint LoadShaders(shaderInfo_t* shaders);
int renderSimple(renderInfo_t renderInfo);
int renderWithCamera(renderInfo_t renderInfo, CameraZ camera);
void initShaderPrograms();
