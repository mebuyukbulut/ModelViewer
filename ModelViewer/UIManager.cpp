#include "UIManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "FileUtils.h"
#include "LightManager.h"

void UIManager::init(GLFWwindow* window, LightManager* lightManager) {
	_window = window;
	_lightManager = lightManager;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void UIManager::terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}

void UIManager::draw() {
	beginFrame();
	mainMenu();
	if(isShaderPanelOpen) shaderPanel();
	if(isCreditsPanelOpen) creditsPanel();
	_lightManager->drawUI();


	endFrame();
}

bool UIManager::isHoverOnUI()
{
    
    return ImGui::IsWindowHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AnyWindow);
     
}

void UIManager::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void UIManager::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::mainMenu(){	
    // Main menu bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) { onEngineExit(); }
            if (ImGui::MenuItem("Open", "Ctrl+O")) { onOpenModel(FileUtils::openFileDialog()); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Shader Panel", nullptr, &isShaderPanelOpen);
            ImGui::MenuItem("Credits Panel", nullptr, &isCreditsPanelOpen);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void UIManager::shaderPanel()
{
    // Shader selection combo box
    const char* items[] = { "normal", "lambertian", "blinn-phong" };
    static int currentItem = 0;

    if (ImGui::Begin("Shader Controls")) {
        if (ImGui::Combo("Shader", &currentItem, items, IM_ARRAYSIZE(items))) {
            onShaderSelected(items[currentItem]);
        }
    }
    ImGui::End();
}

void UIManager::creditsPanel(){
    if (ImGui::Begin("Credits")) {
		ImGui::Text("Model Viewer by Muhammet Esat BUYUKBULUT");
		ImGui::Text("@2025");
    }
    ImGui::End();
}
