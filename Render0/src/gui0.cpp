#include "render0/gui0.hpp"

namespace gz {

	void init(GLFWwindow* window) {

        RZ_TRACE("Setup Dear ImGui context...");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

        RZ_TRACE("...setup Dear imGui style...");
        
		ImGui::StyleColorsDark();

        RZ_TRACE("...setup Platform/Renderer backends");

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(rz::glsl_version);
		
        guiWindowsToShow[DEMO] = true;
        guiWindowsToShow[DEMO2] = false;

        gz::imGuiOn = true;

        RZ_INFO("...imGui iniciado.");
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

    //Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    void showDemo(float* clearColor){
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        
        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &guiWindowsToShow[DEMO]);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &guiWindowsToShow[DEMO2]);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        
        //ImGui::ColorEdit3("clear color", clearColor); // Edit 3 floats representing a color
        ImGui::ColorEdit4("clear color", clearColor); // Edit 4 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    //Show another simple window.
    void showDemo2(){
        ImGui::Begin("Another Window", &guiWindowsToShow[DEMO2], ImGuiWindowFlags_MenuBar);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            guiWindowsToShow[DEMO2] = false;
               
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
                if (ImGui::MenuItem("Close", "Ctrl+W")) { guiWindowsToShow[DEMO2] = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Plot some values
        static float lastFewFPSs[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        
        static int frameCount = 0;
        frameCount %= 10;
        if (frameCount == 0) {
            for (int i = 1; i < IM_ARRAYSIZE(lastFewFPSs); i++) {
                lastFewFPSs[i - 1] = lastFewFPSs[i];
            }
            lastFewFPSs[IM_ARRAYSIZE(lastFewFPSs) - 1] = ImGui::GetIO().Framerate;    
        }
        frameCount++;

        ImGui::PlotLines("FPS", lastFewFPSs, IM_ARRAYSIZE(lastFewFPSs));

        // Display contents in a scrolling region
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
        ImGui::BeginChild("Scrolling");
        for (int n = 0; n < 50; n++)
            ImGui::Text("%04d: Some text", n);
        ImGui::EndChild();

        ImGui::End();
    }

    void showGuiWindows(float* clearColor) {
        if (guiWindowsToShow[DEMO] == true) {
            showDemo(clearColor);
        }

        if (guiWindowsToShow[DEMO2] == true) {
            showDemo2();
        }
    }
}
