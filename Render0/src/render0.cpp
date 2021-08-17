#include "render0/render0.h"
#include "RZ_API/RZ_api.h"

struct windowParams{
    GLFWwindow* window;
    int width = 640;
    int height = 480;
    const char* title = "Viewer0 " SYSTEM_NAME "-" PLATFORM_NAME " " CONFIG_NAME;
} renderWindow ;


namespace renderz {

    int init() {

        RZ_TRACE("Criando janela...");

        /* Initialize the library */
        if (!glfwInit()) {
            RZ_ERROR("GLFW nao iniciou");
            return -1;
        }
        RZ_INFO("GLFW Iniciado");

        /* Create a windowed mode window and its OpenGL context */
        renderWindow.window = glfwCreateWindow(renderWindow.width, renderWindow.height
            , renderWindow.title, NULL, NULL);
        if (!renderWindow.window)
        {
            RZ_ERROR("Nao conseguiu obter janela");
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(renderWindow.window);

        RZ_INFO("\nJanela criada!");

        return 1;
    }


    int render() {
                
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*Define triangle*/
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(renderWindow.window);

        /* Poll for and process events */
        glfwPollEvents();

        return !glfwWindowShouldClose(renderWindow.window);
    }

    void terminate() {
        glfwTerminate();
        RZ_TRACE("GLFW fechado");
    }
}