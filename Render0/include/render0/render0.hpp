#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "logAPI.hpp"

//#ifndef RENDER_WINDOW
    typedef struct windowParams_st {
        GLFWwindow* window;
        int width = 640;
        int height = 480;
        const char* title = "Viewer0 " SYSTEM_NAME "-" PLATFORM_NAME " " CONFIG_NAME;
    } windowParams_t;

    inline windowParams_t renderWindow;

//    #define RENDER_WINDOW
//#endif
