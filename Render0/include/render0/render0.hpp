#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "RZ_API/RZ_api.hpp"
#include "logAPI.hpp"

//#ifndef RENDER_GLOBALS
    
    #define BUFFER_OFFSET(a) ((void*)(a))

    enum VAO_IDs { Triangles, NumVAOs };
    enum Buffer_IDs { ArrayBuffer, NumBuffers };

    typedef struct windowParams_st {
        GLFWwindow* window;
        int width = 640;
        int height = 480;
        const char* title = "Viewer0 " SYSTEM_NAME "-" PLATFORM_NAME " " CONFIG_NAME;
    } windowParams_t;

    inline windowParams_t renderWindow;

    typedef struct renderInfo_st {
        rz::colorf_t clearColor;
        GLuint VAO;
        GLuint numVertices;
        GLFWwindow* window;
    } renderInfo_t;

    typedef struct vertexDataCP_st {
        GLubyte color[4];
        GLfloat position[4];
    } vertexDataCP_t;
    enum Attrib_IDs { vPosition = 0, vColor = 1 };

    typedef struct {
        GLenum       type;
        const char* filename;
        GLuint       shader;
    } shaderInfo_t;

//    #define RENDER_GLOBALS
//#endif

GLuint LoadShaders(shaderInfo_t* shaders);
renderInfo_t getTestTriangleInfo(void);
int renderSimple(renderInfo_t renderInfo);
