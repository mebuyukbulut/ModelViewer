#include "UIManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


#include "FileUtils.h"
#include "LightManager.h"
#include "Camera.h"
#include "Material.h"
#include <iostream>

#include "EventDispatcher.h"
#include "Config.h"

#include "ParticleSystem.h"

void UIManager::init(GLFWwindow* window, LightManager* lightManager, std::shared_ptr<Camera> camera) {
	_window = window;
	_lightManager = lightManager;
    _camera = camera;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();


    ImGuiIO& io = ImGui::GetIO();
    // Türkçe karakter aralığını ekleyelim
    static const ImWchar ranges[] = {
        0x0020, 0x00FF, // Latin + Latin Supplement
        0x0100, 0x017F, // Latin Extended-A (Türkçe karakterler burada)
        0
    };
    //// Yeni font yükleme (örnek: Roboto)
    //io.Fonts->AddFontFromFileTTF("fonts/Roboto-VariableFont_wdth,wght.ttf", 16.0f, NULL, ranges);

    // Eğer default fontu da korumak istersen:
    io.FontDefault = io.Fonts->AddFontFromFileTTF("fonts/Roboto-VariableFont_wdth,wght.ttf", 16.0f, NULL, ranges);


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
        
    // Son olarak font atlasını yeniden yükle
    ImGui_ImplOpenGL3_CreateFontsTexture();


    // load config 
    isCreditsPanelOpen = config.ui.isCreditsPanelOpen;
    isLightPanelOpen = config.ui.isLightPanelOpen;
    isMaterialPanelOpen = config.ui.isMaterialPanelOpen;
    isShaderPanelOpen = config.ui.isShaderPanelOpen;

}

void UIManager::terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}

void UIManager::draw(Material* material) {
	beginFrame();
	mainMenu();
    ps->drawUI();
	if(isShaderPanelOpen) shaderPanel();
	if(isCreditsPanelOpen) creditsPanel();
    if (isLightPanelOpen) {
	    _lightManager->drawUI();
        _lightManager->drawGizmo();
    }
    if(isMaterialPanelOpen) material->drawUI();

    //ImGui::ShowDemoWindow();
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
            if (ImGui::MenuItem("Exit")) { 
                Event e{ EventType::EngineExit, EventData{} };
                dispatcher.dispatch(e);
            }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                EventData ed{};
                ed.text = FileUtils::openFileDialog();
                Event e{ EventType::ModelOpened, ed };
                dispatcher.dispatch(e);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Shader Panel", nullptr, &isShaderPanelOpen)) {
                config.ui.isShaderPanelOpen = isShaderPanelOpen;
                config.save();
            }
            if(ImGui::MenuItem("Credits Panel", nullptr, &isCreditsPanelOpen)) {
                config.ui.isCreditsPanelOpen = isCreditsPanelOpen;
                config.save();
            }
            if(ImGui::MenuItem("Light Panel", nullptr, &isLightPanelOpen)) {
                config.ui.isLightPanelOpen = isLightPanelOpen;
                config.save();
            }
            if(ImGui::MenuItem("Material Panel", nullptr, &isMaterialPanelOpen)) {
                config.ui.isMaterialPanelOpen = isMaterialPanelOpen;
                config.save();
            }
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
            EventData ed{};
            ed.text = items[currentItem];
            Event e{ EventType::ShaderSelected, ed };
            dispatcher.dispatch(e);
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
