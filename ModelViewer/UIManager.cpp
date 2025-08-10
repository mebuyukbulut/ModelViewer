#include "UIManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


void UIManager::init(GLFWwindow* window) {
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
    // UI logic here


    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {}
            if (ImGui::MenuItem("Create")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save as..")) {}
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }


    const char* items[] = { "normal", "lambertian" };
    static int currentItem = 0;

    if (ImGui::Begin("Shader Controls")) {
        if (ImGui::Combo("Shader", &currentItem, items, IM_ARRAYSIZE(items))) {
			onShaderSelected(items[currentItem]);
        }
    }
    ImGui::End();
    //ImGui::Begin("Shader Control");
    //if (ImGui::Button("Use Diffuse"));

    //ImGui::End();
	endFrame();
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