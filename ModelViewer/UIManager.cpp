#include "UIManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileUtils.h"
#include "LightManager.h"
#include "Camera.h"

void UIManager::init(GLFWwindow* window, LightManager* lightManager, Camera* camera) {
	_window = window;
	_lightManager = lightManager;
    _camera = camera;

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





    // Make sure to call inside ImGui frame:
    ImGuizmo::BeginFrame();

    // Get viewport size
    ImVec2 windowPos(0, 0);// = ImGui::GetWindowPos();
    //ImVec2 windowSize(600, 600);// = ImGui::GetWindowSize();

    // Setup ImGuizmo rect
    ImGuizmo::SetRect(windowPos.x, windowPos.y, _windowSize.x, _windowSize.y);

    //// Example: use your camera/view/projection matrices
    //float view[16];       // from your camera
    //float projection[16]; // from your camera
    //float objectMatrix[16]; // model matrix of the object you want to manipulate

    //// Convert glm::mat4 to float[16] if you’re using glm
    //memcpy(view, glm::value_ptr(viewMat), sizeof(float) * 16);
    //memcpy(projection, glm::value_ptr(projMat), sizeof(float) * 16);
    //memcpy(objectMatrix, glm::value_ptr(modelMat), sizeof(float) * 16);

    // Choose operation: translate / rotate / scale
    static ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

    glm::mat4 modelMatrix(1); 
    ImGuizmo::Manipulate(
        glm::value_ptr(_camera->getViewMatrix()),
        glm::value_ptr(_camera->getProjectionMatrix()),
        mCurrentGizmoOperation,
        ImGuizmo::LOCAL,
        glm::value_ptr(modelMatrix)
    );


    //// Manipulate the matrix in place
    //ImGuizmo::Manipulate(glm::value_ptr(vM), projection, mCurrentGizmoOperation,
    //    ImGuizmo::LOCAL, objectMatrix);

    //// If modified, write back into your glm::mat4
    //if (ImGuizmo::IsUsing()) {
    //    modelMat = glm::make_mat4(objectMatrix);
    //}





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
