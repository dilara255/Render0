#include "render0/gui0.hpp"

namespace gz {

    static guiControlVariables_st controlVariables;

    void initControlVariables();

	void init(GLFWwindow* window, renderInfo_t* renderInfo_ptr, CameraZ* camera_ptr) {

        RZ_TRACE("Setup Dear ImGui context...");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

        RZ_TRACE("...setup Dear imGui style...");
        
		ImGui::StyleColorsDark();

        RZ_TRACE("...setup Platform/Renderer backends");

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(rz::glsl_version);
		
        guiWindowsToShow[GUI_MAIN] = true;
        guiWindowsToShow[GUI_STATS] = true;

        gz::imGuiOn = true;

        RZ_INFO("...imGui iniciado.");

        controlVariables.renderInfo_ptr = renderInfo_ptr;
        controlVariables.camera_ptr = camera_ptr;

        initControlVariables();
	}

    void updateControlVariables() {

        controlVariables.backfaceCull.currentState = controlVariables.renderInfo_ptr->shouldCull;

        controlVariables.useLookAt.currentState = controlVariables.camera_ptr->lookAtIsOn();

        controlVariables.swapInterval = controlVariables.renderInfo_ptr->swapInterval;
        controlVariables.useVsync.currentState = (controlVariables.swapInterval > 0);

        controlVariables.orientation =
            controlVariables.renderInfo_ptr->faceDirectionForCulling;
        
        controlVariables.faceOrientationArr[0].currentState =
            (controlVariables.orientation == GL_CW);
       
        controlVariables.faceOrientationArr[1].currentState =
            (controlVariables.orientation == GL_CCW);

        controlVariables.nearDist = -controlVariables.camera_ptr->getNearDist();
        controlVariables.farDist = -controlVariables.camera_ptr->getFarDist();

        controlVariables.fovH = glm::degrees(controlVariables.camera_ptr->getFovHor());
        controlVariables.fovV = glm::degrees(controlVariables.camera_ptr->getFovVer());

        controlVariables.projectionArr[0].currentState =
            controlVariables.camera_ptr->perspectiveIsOn();
        
        controlVariables.projectionArr[1].currentState =
            (!controlVariables.camera_ptr->perspectiveIsOn());
        
        controlVariables.rendererArr[0].currentState = true;
        controlVariables.rendererArr[1].currentState = false;
        
        controlVariables.shaderArr[0].currentState = 
            controlVariables.renderInfo_ptr->shadingToUse == 0;
        controlVariables.shaderArr[1].currentState =
            controlVariables.renderInfo_ptr->shadingToUse == 1;
        controlVariables.shaderArr[2].currentState =
            controlVariables.renderInfo_ptr->shadingToUse == 2;
        controlVariables.shaderArr[3].currentState =
            controlVariables.renderInfo_ptr->shadingToUse == 3;
        controlVariables.shaderArr[4].currentState =
            controlVariables.renderInfo_ptr->shadingToUse == 4;
    }

    void updateLastFrameControlVariables() {

        controlVariables.useLookAt.lastState = controlVariables.useLookAt.currentState;
        controlVariables.backfaceCull.lastState = controlVariables.backfaceCull.currentState;
        controlVariables.useVsync.lastState = controlVariables.useVsync.currentState;

        controlVariables.faceOrientationArr[0].lastState =
            controlVariables.faceOrientationArr[0].currentState;
        controlVariables.faceOrientationArr[1].lastState =
            controlVariables.faceOrientationArr[1].currentState;

        controlVariables.projectionArr[0].lastState = controlVariables.projectionArr[0].currentState;
        controlVariables.projectionArr[1].lastState = controlVariables.projectionArr[1].currentState;

        controlVariables.rendererArr[0].lastState = true;
        controlVariables.rendererArr[1].lastState = false;

        controlVariables.shaderArr[0].lastState = false;
        controlVariables.shaderArr[1].lastState = false;
        controlVariables.shaderArr[2].lastState = true;
        controlVariables.shaderArr[3].lastState = false;
        controlVariables.shaderArr[4].lastState = false;
    }

    void initControlVariables() {
        RZ_TRACE("Iniciando variáveis de controle da GUI...");

        updateControlVariables();
        updateLastFrameControlVariables();     

        RZ_INFO("...variáveis de controle da GUI iniciadas.");
    }

    void startGuiFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void renderGui() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void terminate() {
        RZ_TRACE("Closing imGui...");

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        gz::imGuiOn = false;

        RZ_INFO("...imGui fechado.");
    }

    void toggleOptions(toggle_st toggleInfo) {
    //cria botões mutuamente excludentes - EXATAMENTE um deve estar sempre marcado
    //tô fazendo o mesmo loop várias vezes, tem que revisar isso

        ImGui::Text(toggleInfo.description);
        ImGui::SameLine();

        for (int i = 0; i < toggleInfo.numberOptions; i++) {
            if(i % 3 != 2) ImGui::SameLine();
            ImGui::Checkbox(toggleInfo.optionDescriptionsArray[i],
                &toggleInfo.optionsArray[i].currentState);            
        }

        int count = 0;
        for (int i = 0; i < toggleInfo.numberOptions; i++) {

            //não "desliga" clicando numa caixa ligada:
            if (toggleInfo.optionsArray[i].currentState == false) {
                count++;
            }
        }

        if (count == toggleInfo.numberOptions) {
            for (int i = 0; i < toggleInfo.numberOptions; i++) {
                if (!toggleInfo.optionsArray[i].currentState
                    && toggleInfo.optionsArray[i].lastState) {

                    toggleInfo.optionsArray[i].currentState = true;
                }
            }
        }
        else {

            for (int i = 0; i < toggleInfo.numberOptions; i++) {
                //se ligou em uma caixa, desliga todas outras:
                if (toggleInfo.optionsArray[i].currentState
                    && !toggleInfo.optionsArray[i].lastState) {

                    for (int j = 0; j < toggleInfo.numberOptions; j++) {
                        if (j != i) toggleInfo.optionsArray[j].currentState = false;
                    }
                }
            }
        }

    }
    
    void mainGui(){
        
        ImGui::Begin("Control Panel");
        ImGui::Checkbox("Statistics Window", &guiWindowsToShow[GUI_STATS]);

        ImGui::SameLine(); ImGui::Text("                "); ImGui::SameLine();
        if (ImGui::Button("Exit")) {
            glfwSetWindowShouldClose(controlVariables.renderInfo_ptr->window, GL_TRUE);
        }

        ImGui::Text("Opcoes do Render");               // Display some text (you can use a format strings too)
           
        ImGui::ColorEdit3("Clear", controlVariables.renderInfo_ptr->clearColor);
        ImGui::ColorEdit4("Model", &controlVariables.renderInfo_ptr->colorForUniform.r);

        ImGui::SliderFloat("Vert. FoV", &controlVariables.fovV, 0.1f, 179.9f);
        ImGui::SliderFloat("Hor. FoV", &controlVariables.fovH, 0.1f, 179.9f);

        ImGui::SliderFloat("Near Dist", &controlVariables.nearDist, 
            MIN_NEAR, controlVariables.farDist - MIN_NEAR);
        ImGui::SliderFloat("Far Dist", &controlVariables.farDist, 
            controlVariables.nearDist + MIN_NEAR, MAX_FAR);

        ImGui::Checkbox("LookAt Camera", &controlVariables.useLookAt.currentState);

        ImGui::SameLine();

        if (ImGui::Button("Reset Camera")) {
            controlVariables.camera_ptr->resetCamera();
            controlVariables.camera_ptr->updateCameraMatrix();
        }

        ImGui::Checkbox("Backface Culling", &controlVariables.backfaceCull.currentState);

        ImGui::SameLine();

        ImGui::Checkbox("Vsync", &controlVariables.useVsync.currentState);
        
        toggleOptions(controlVariables.faceOrientation);
        if (controlVariables.faceOrientationArr[0].currentState != controlVariables.faceOrientationArr[0].lastState) {
            printf("\n%i %i %i %i",
                controlVariables.faceOrientationArr[0].currentState,
                controlVariables.faceOrientationArr[0].lastState,
                controlVariables.faceOrientationArr[1].currentState,
                controlVariables.faceOrientationArr[1].lastState);
        }
        toggleOptions(controlVariables.projection);
        toggleOptions(controlVariables.renderer);
        toggleOptions(controlVariables.shader);

        ImGui::End();
    }

    //Show another simple window.
    void statisticsGui(){
        ImGui::Begin("Statistics Window", &guiWindowsToShow[GUI_STATS]);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                       
        float fps = ImGui::GetIO().Framerate;
        controlVariables.renderInfo_ptr->frameTimeMS = 1000.0f / fps;

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                     1000.0f / fps, fps);

        const int numberFramesToKeepFps = 6;
        static float lastFewFPSs[numberFramesToKeepFps +2] = 
            { 60.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        
        static int frameCount = 0;
        frameCount %= (int)ceil(fps/numberFramesToKeepFps);
        if (frameCount == 0) { //tentar atualizar grafico todo 1 vez por segundo
            for (int i = 3; i < IM_ARRAYSIZE(lastFewFPSs); i++) {
                lastFewFPSs[i - 1] = lastFewFPSs[i];
            }
            lastFewFPSs[IM_ARRAYSIZE(lastFewFPSs) - 1] = fps;
            if (lastFewFPSs[IM_ARRAYSIZE(lastFewFPSs) - 1] > lastFewFPSs[0]) {
                lastFewFPSs[0] = lastFewFPSs[IM_ARRAYSIZE(lastFewFPSs) - 1];
            }
        }
        frameCount++;

        ImGui::PlotLines("FPS", lastFewFPSs, IM_ARRAYSIZE(lastFewFPSs));

        ImGui::End();
    }

    void showGuiWindows() {
        
        updateLastFrameControlVariables();
        updateControlVariables();
                
        if (guiWindowsToShow[GUI_MAIN] == true) {
            mainGui();
        }

        if (guiWindowsToShow[GUI_STATS] == true) {
            statisticsGui();
        }
    }

    void updateBoolState(bool currentGuiState, bool* lastGuiState_ptr, bool* stateState_ptr) {
        if (currentGuiState != *lastGuiState_ptr) {
            *stateState_ptr = currentGuiState;
            *lastGuiState_ptr = currentGuiState;
        }
    }

    void updateStateFromGui() {

        updateBoolState(controlVariables.backfaceCull.currentState,
            &controlVariables.backfaceCull.lastState,
            &controlVariables.renderInfo_ptr->shouldCull);

        static bool lookAtShouldBeOn = controlVariables.useLookAt.currentState;
        static bool lookAtShouldBeOnLastFrame = controlVariables.useLookAt.lastState;
        updateBoolState(controlVariables.useLookAt.currentState,
            &controlVariables.useLookAt.lastState,
            &lookAtShouldBeOn);
        if (lookAtShouldBeOn != lookAtShouldBeOnLastFrame) {
            if (lookAtShouldBeOn) {
                controlVariables.camera_ptr->lookAtTurnOn();
            }
            else controlVariables.camera_ptr->lookAtTurnOff();

            lookAtShouldBeOnLastFrame = lookAtShouldBeOn;
        }

        static bool vsyncShouldBeOn = controlVariables.useVsync.currentState;
        static bool vsyncShouldBeOnLastFrame = controlVariables.useVsync.lastState;
        updateBoolState(controlVariables.useVsync.currentState,
            &controlVariables.useVsync.lastState,
            &vsyncShouldBeOn);
        if (vsyncShouldBeOn != vsyncShouldBeOnLastFrame) {

            if (vsyncShouldBeOn) controlVariables.renderInfo_ptr->swapInterval = 1;
            else controlVariables.renderInfo_ptr->swapInterval = 0;

            rz::setSwapInterval(controlVariables.renderInfo_ptr->swapInterval);

            vsyncShouldBeOnLastFrame = vsyncShouldBeOn;
        }

        static bool shouldBeCW = controlVariables.faceOrientationArr[0].currentState;
        static bool shouldBeCWLastFrame = controlVariables.faceOrientationArr[0].lastState;
        updateBoolState(controlVariables.faceOrientationArr[0].currentState,
            &controlVariables.faceOrientationArr[0].lastState,
            &shouldBeCW);
        
        if (shouldBeCW != shouldBeCWLastFrame){
            if(shouldBeCW) controlVariables.renderInfo_ptr->faceDirectionForCulling = GL_CW;
            else controlVariables.renderInfo_ptr->faceDirectionForCulling = GL_CCW;

            shouldBeCWLastFrame = shouldBeCW;
        }

        static bool shouldBePerspective = controlVariables.projectionArr[0].currentState;
        static bool shouldBePerspectiveLastFrame = controlVariables.projectionArr[0].lastState;
        updateBoolState(controlVariables.projectionArr[0].currentState,
            &controlVariables.projectionArr[0].lastState,
            &shouldBePerspective);

        if (shouldBePerspective != shouldBePerspectiveLastFrame) {
            if (shouldBePerspective) controlVariables.camera_ptr->perspectiveTurnOn();
            else controlVariables.camera_ptr->perspectiveTurnOff();

            shouldBePerspectiveLastFrame = shouldBePerspective;
        }

        controlVariables.camera_ptr->changeNearDistBy(
            controlVariables.nearDist - (-controlVariables.camera_ptr->getNearDist()) );
        controlVariables.camera_ptr->changeFarDistBy(
            controlVariables.farDist - (-controlVariables.camera_ptr->getFarDist()) );
            
        controlVariables.camera_ptr->setFovHor(glm::radians(controlVariables.fovH));
        controlVariables.camera_ptr->setFovVer(glm::radians(controlVariables.fovV));

        controlVariables.camera_ptr->updateCameraMatrix();
    }
}
