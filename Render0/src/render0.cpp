/*
* Inicialização geral do renderer e dependências e encerramento GLFW. 
* Funções de renderização com/sem câmera e modelo. Criação de estado para renderização.
* Callback de erro de glfw e função de checagem de erros de OpenGL
* 
* Depndendo do modo de render, LOOPING PRINCIPAL ainda ESTÁ AQUI, dentro das funções de render. 
* Ideia é tirar isso daqui pra todos modos.
*/

#include "render0/render0.hpp"
#include "render0/model0.hpp"
#include "render0/gui0.hpp"
#include "RZ_API/RZ_api.hpp"

void error_callback(int error, const char* description);
void checkOGlErrors();

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

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

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

        RZ_TRACE("Iniciando imGui..");

        gz::init(renderWindow.window);

        RZ_INFO("\n\nPronto!");

        return 1;
    }

    void terminate() {
        if(gz::imGuiOn) gz::terminate();
        glfwTerminate();
        RZ_TRACE("GLFW fechado");
    }
}

bool render(renderInfo_t* renderInfo_ptr, CameraZ* camera_ptr, float* modelMatrixStart_ptr,
            void (*controlTest) (CameraZ* camera_ptr, renderInfo_t* renderInfo_ptr)) {
    
    static float clearToThisDepth = 1.0f;
    bool keepRendering = !glfwWindowShouldClose(renderInfo_ptr->window);

    glUniform4fv(renderInfo_ptr->uniformLocations[vColorUniform], 1,
        &renderInfo_ptr->colorForUniform.r);
    glUniformMatrix4fv(renderInfo_ptr->uniformLocations[vPV_matrix], 1,
        false, &camera_ptr->cameraProjection[0][0]);
    glUniformMatrix4fv(renderInfo_ptr->uniformLocations[vModel_matrix], 1,
        false, modelMatrixStart_ptr);

    if (renderInfo_ptr->shouldCull) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(renderInfo_ptr->faceDirectionForCulling);
    }
    else glDisable(GL_CULL_FACE);

    glClearBufferfv(GL_COLOR, 0, renderInfo_ptr->clearColor);
    glClearBufferfv(GL_DEPTH, 0, &clearToThisDepth);

    glBindVertexArray(renderInfo_ptr->VAO);

    glDrawArrays(renderInfo_ptr->modes[renderInfo_ptr->mode], 0, renderInfo_ptr->numVertices);

    gz::startGuiFrame();
    gz::showGuiWindows(renderInfo_ptr->clearColor);
    gz::renderGui();

    glfwSwapBuffers(renderInfo_ptr->window);

    glfwPollEvents();

    controlTest(camera_ptr, renderInfo_ptr);

    return keepRendering;
}

void setupRender(renderInfo_t* renderInfo, int shader) {
    RZ_TRACE("Preparando para renderizar. Carregar shaders...");

    shaderPrograms[shader].shaderProgramId = LoadShaders(shaderPrograms[shader].shaderInfo);

    RZ_TRACE("Ligar programa...");
    glUseProgram(shaderPrograms[shader].shaderProgramId);

    RZ_TRACE("Ligar Depth Test...");
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    RZ_TRACE("Setar localizacao Uniforms...");

    renderInfo->uniformLocations[vColorUniform] = glGetUniformLocation(
        shaderPrograms[shader].shaderProgramId
        , "vColorUniform");

    renderInfo->uniformLocations[vPV_matrix] = glGetUniformLocation(
        shaderPrograms[shader].shaderProgramId
        , "vPV_matrix");

    renderInfo->uniformLocations[vModel_matrix] = glGetUniformLocation(
        shaderPrograms[shader].shaderProgramId
        , "vModel_matrix");

    glfwSwapInterval(1);

    RZ_TRACE("Pronto. Checando erros...");

    checkOGlErrors();
}

renderInfo_t setupRenderInfoCameraModelSimple(CameraZ* camera_ptr, mz::ModelZ* model_ptr) {

    RZ_TRACE("Renderizar modelo lido de arquivo com material...");

    GLuint  VAOs[NumVAOs];
    GLuint  Buffers[NumBuffersWithModels]; //NumBuffersWithModels //AQUI

    int numTriangles = model_ptr->getNumberTriangles();
    int numMaterials = model_ptr->getNumberMaterials();

    GLuint  numVertices = 3 * numTriangles;

    RZ_TRACE("Gerando vertex array e buffers...");

    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    glGenBuffers(NumBuffersWithModels, Buffers); //AQUI

    RZ_TRACE("Preparando Buffer Posicao...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4)
        , model_ptr->posBuffer
        , GL_STATIC_DRAW);

    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);

    //AQUI
    RZ_TRACE("Preparando Buffer Cor Difusa...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[DiffuseBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4)
        , model_ptr->diffuseBuffer
        , GL_STATIC_DRAW);

    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vColor);
    //AQUI

    RZ_TRACE("Preparando Buffer Normal...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[NormBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4)
        , model_ptr->normBuffer
        , GL_STATIC_DRAW);

    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vNormal);

    //AQUI
    RZ_TRACE("Preparando Buffer Ambiente...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[AmbientBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4)
        , model_ptr->ambientBuffer
        , GL_STATIC_DRAW);

    glVertexAttribPointer(vAmbient, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vAmbient);

    RZ_TRACE("Preparando Buffer Especular...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[SpecularBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec4)
        , model_ptr->specularBuffer
        , GL_STATIC_DRAW);

    glVertexAttribPointer(vSpecular, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vSpecular);

    RZ_TRACE("Preparando Buffer Coef Brilho Especular...");

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[SpcCoefBuffer]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float)
        , model_ptr->spcShineCoefBuffer
        , GL_STATIC_DRAW);

    glVertexAttribPointer(vSpcCoef, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vSpcCoef);
    //AQUI

    RZ_TRACE("Pronto. Definindo cor de fundo/clear e juntando as infos pra renderizar...");

    const rz::colorf_t black = { 0.0f, 0.0f, 0.0f, 0.0f };

    renderInfo_t renderInfo = {
        black[0],
        black[1],
        black[2],
        black[3],
        VAOs[Triangles],
        numVertices,
        renderWindow.window,
        0,
        camera_ptr,
        model_ptr
    };

    return renderInfo;
}

void error_callback(int error, const char* description)
{
    RZ_ERROR(description);
    printf("\n\t( error id: %i )", error);
}

void checkOGlErrors() {
    RZ_WARN("Checando se tivemos algum erro de OpenGL ateh aqui...\n");

    int errors;
    int errorCount = 0;

    while ((errors = glGetError()) != GL_NO_ERROR) {
        errorCount++;
        printf("\t-> ERRO OPENGL: %i\n", errors);
    }

    if (errorCount > 0) RZ_ERROR("\n\nOuveram erros de OpenGL desde a ultima checagem, veja codigos acima!\n\n");
    else RZ_INFO("\nNao houveram erros de OpenGL desde a ultima checagem : )\n");
}


///////////////////////////////////////////////
///////////////////////////////////////////////
//Other Rendering modes, to be cleaned up soon:
///////////////////////////////////////////////

int rz::renderTestLegacy() {

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

int renderWithCamera(renderInfo_t* renderInfo, CameraZ camera
    , void (*controlTestCamera) (CameraZ* camera)) {

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

    GLint vColorUniformLocation = glGetUniformLocation(shaderPrograms[shader].shaderProgramId
        , "vColorUniform");

    GLint vPV_matrixUniformLocation = glGetUniformLocation(shaderPrograms[shader].shaderProgramId
        , "vPV_matrix");

    GLint vModel_matrixUniformLocation = glGetUniformLocation(shaderPrograms[shader].shaderProgramId
        , "vModel_matrix");

    glfwSwapInterval(1);

    checkOGlErrors();

    while (!glfwWindowShouldClose(renderInfo->window)) {
       
        glUniform4fv(vColorUniformLocation, 1, &renderInfo->colorForUniform.r);
        glUniformMatrix4fv(vPV_matrixUniformLocation, 1, false, &camera.cameraProjection[0][0]);
        glUniformMatrix4fv(vModel_matrixUniformLocation, 1, false, &modelMatrix[0][0]);

        if (renderInfo->shouldCull) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(renderInfo->faceDirectionForCulling);
        }
        else glDisable(GL_CULL_FACE);

        glClearBufferfv(GL_COLOR, 0, renderInfo->clearColor);
        glClearBufferfv(GL_DEPTH, 0, &clearToThisDepth);
        
        glBindVertexArray(renderInfo->VAO);

        glDrawArrays(renderInfo->modes[renderInfo->mode], 0, renderInfo->numVertices);

        gz::startGuiFrame();
        gz::showGuiWindows(renderInfo->clearColor);
        gz::renderGui();

        glfwSwapBuffers(renderInfo->window);

        glfwPollEvents();

        controlTest(&camera, renderInfo);
    }

    checkOGlErrors();

    rz::terminate();
    RZ_INFO("Janela Fechada");

    return 0;
}