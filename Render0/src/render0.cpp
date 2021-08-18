#include "render0/render0.hpp"
#include "RZ_API/RZ_api.hpp"

namespace rz {

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

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
        {
            RZ_ERROR("Nao conseguiu carregar openGL");
            glfwTerminate();
            return -1;
        }

        RZ_INFO("\nOpenGL carregado!");

        return 1;
    }


    int renderTest0() {
                
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

int renderSimple(renderInfo_t renderInfo) {

    RZ_INFO("Habilitando atributos vColor e vPosition...");

    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vPosition);

    RZ_INFO("Renderizar...");

    while (!glfwWindowShouldClose(renderInfo.window))
    {
        glClearBufferfv(GL_COLOR, 0, renderInfo.clearColor);

        glBindVertexArray(renderInfo.VAO);

        glDrawArrays(GL_TRIANGLES, 0, renderInfo.numVertices);

        glfwSwapBuffers(renderInfo.window);

        glfwPollEvents();
    }

    RZ_INFO("Chega de triangulo...");

    rz::terminate();

    return 1;
}