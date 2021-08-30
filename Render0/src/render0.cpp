#include "render0/render0.hpp"
#include "render0/model0.hpp"
#include "RZ_API/RZ_api.hpp"

void error_callback(int error, const char* description);

namespace rz {

    int init() {

        //TESTE
        //mz::ModelZ model("models/Teste3DMax.max");//TESTE
        //TESTE

        RZ_TRACE("Criando janela...");

        if (!glfwInit()) {
            RZ_ERROR("GLFW nao iniciou");
            return -1;
        }
        RZ_INFO("GLFW Iniciado");

        RZ_TRACE("Setar Callback de erro e tentar criar janela...");

        glfwSetErrorCallback(error_callback);

        renderWindow.window = glfwCreateWindow(renderWindow.width, renderWindow.height
            , renderWindow.title, NULL, NULL);
        if (!renderWindow.window)
        {
            RZ_ERROR("Nao conseguiu obter janela");
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(renderWindow.window);

        RZ_INFO("\nJanela criada!");

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
        {
            RZ_ERROR("Nao conseguiu carregar openGL");
            glfwTerminate();
            return -1;
        }

        RZ_INFO("\nOpenGL carregado!");
        RZ_TRACE("Vai preparar programas de shader..");

        initShaderPrograms();

        RZ_INFO("Shaders preparados");

        iz::init(renderWindow.window);

        return 1;
    }

    void terminate() {
        glfwTerminate();
        RZ_TRACE("GLFW fechado");
    }

    int renderTestLegacy() {

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

        glDrawArrays(renderInfo.mode, 0, renderInfo.numVertices);

        glfwSwapBuffers(renderInfo.window);

        glfwPollEvents();
    }

    RZ_INFO("Chega de triangulo...");

    rz::terminate();

    return 1;
}

int renderWithCamera( renderInfo_t *renderInfo, CameraZ camera
                    , void (*controlTestCamera) (CameraZ *camera)) {

    RZ_INFO("Habilitando atributos vColor, vPosition e Matriz Projecao...");

    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vPosition);
    
    glm::int32_t projectionLocation = glGetUniformLocation(shaderPrograms[SIMPLE_CAMERA].shaderProgramId
                                                          , "vProjection");
    
    printf("\t\tLocalizacao uniform projecao: %i\n", projectionLocation);

    RZ_INFO("Renderizar...");

    while (!glfwWindowShouldClose(renderInfo->window))
    {
        glUniformMatrix4fv(projectionLocation, 1, false, &camera.cameraProjection[0][0]);

        glClearBufferfv(GL_COLOR, 0, renderInfo->clearColor);

        glBindVertexArray(renderInfo->VAO);

        glDrawArrays(renderInfo->mode, 0, renderInfo->numVertices);

        glfwSwapBuffers(renderInfo->window);

        glfwPollEvents();

        controlTestCamera(&camera);
    }

    RZ_INFO("Chega de triangulo...");

    rz::terminate();

    return 1;
}


int renderWithCameraAndModel(renderInfo_t* renderInfo, CameraZ camera, glm::mat4 modelMatrix
    , int shader, void (*controlTest) (CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr)) {
    
    RZ_TRACE("Preparando para renderizar. Carregar shaders...");

    shaderPrograms[shader].shaderProgramId = LoadShaders(shaderPrograms[shader].shaderInfo);
    RZ_TRACE("Ligar programa...");
    glUseProgram(shaderPrograms[shader].shaderProgramId);

    glm::mat4 modelMatrixRender = modelMatrix;

    
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    float clearToThisDepth = 1.0f;

    while (!glfwWindowShouldClose(renderInfo->window)) {

        if (renderInfo->shouldCull) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(renderInfo->faceDirectionForCulling);
        }
        else glDisable(GL_CULL_FACE);
        
        glUniformMatrix4fv(vPV_matrix, 1, false, &camera.cameraProjection[0][0]);
        glUniformMatrix4fv(vModel_matrix, 1, false, &modelMatrix[0][0]);
        
        glClearBufferfv(GL_COLOR, 0, renderInfo->clearColor);
        glClearBufferfv(GL_DEPTH, 0, &clearToThisDepth);
        
        glBindVertexArray(renderInfo->VAO);

        glDrawArrays(renderInfo->modes[renderInfo->mode], 0, renderInfo->numVertices);

        glfwSwapBuffers(renderInfo->window);

        glfwPollEvents();

        controlTest(&camera, renderInfo);
    }

    RZ_TRACE("Checando se tivemos algum erro...\n");

    int errors;
    int errorCount = 0;

    while( (errors = glGetError()) != GL_NO_ERROR){
        errorCount++;
        printf("\t-> ERRO OPENGL: %i\n", errors);
    }

    if (errorCount > 0) RZ_WARN("\n\nOuveram erros de OpenGL durante a execucao, veja codigos acima!\n\n");
    else RZ_INFO("\nNao houveram erros de OpenGL durante a execucao : )\n");

    rz::terminate();
    RZ_INFO("Janela Fechada");

    return 0;
}

void error_callback(int error, const char* description)
{
    RZ_ERROR(description);
    printf("\n\t( error id: %i )", error);
}