#include "SceneManager.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "LightManager.h"

void SceneManager::drawUI()
{
    ImGui::Begin("Scene");
    ImGuiTreeNodeFlags flag = 
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_Bullet |
        ImGuiTreeNodeFlags_Leaf;
    ImGuiTreeNodeFlags flag2 =  ImGuiTreeNodeFlags_Leaf;
    ImGuiTreeNodeFlags selectedFlag =  ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Selected;
    //ImGuiTreeNodeFlags selectedFlag =  ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
    if (ImGui::IsItemClicked())
    {
        std::cout << " scene window was clicked \n";
        // Mark rendered node as being clicked
    }
    if (ImGui::TreeNodeEx("scene", flag))
    {
        if (ImGui::IsItemClicked())
        {
            std::cout << " I am clicked \n"; 
            // Mark rendered node as being clicked
        }
        // Call ImGui::TreeNodeEx() recursively to populate each level of children

        // gray background
        ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(55, 55, 55, 255));
        for (auto&& i : _transforms) {
            bool lastSelected = false;
            if (i.get() == getSelectedTransform()) {
                lastSelected = true;
                // red foreground
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(230, 125, 15, 255));
                //// gray background
                //ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(65, 65, 65, 255));
            }
            ImGui::TreeNodeEx(i->name.c_str(), i->isSelected() ? selectedFlag : flag2); 
            if (lastSelected)
                ImGui::PopStyleColor(1);

            if (ImGui::IsItemClicked())
            {
                _selectedTransforms.push_back(i.get());
                _selectedTransform = i.get();

                _selectedTransforms.sort();
                _selectedTransforms.unique();
                //std::cout << _selectedTransforms.size() << std::endl; 
                i->select();
            }
            ImGui::TreePop();
        }

        ImGui::PopStyleColor(1);


        ////fmt::format()
        //ImGui::TreeNodeEx("1bc", flag2); ImGui::TreePop();
        //if (ImGui::TreeNodeEx("2bc")) { 
        //    ImGui::TreeNodeEx("0bc", flag2); ImGui::TreePop();
        //    ImGui::TreeNodeEx("1bc", flag2); ImGui::TreePop();
        //    ImGui::TreePop();
        //}

        ImGui::TreePop();  // This is required at the end of the if block
    }


    // bos alana tiklamayi yakala:
    if (ImGui::IsWindowFocused() &&
        ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
        !ImGui::IsAnyItemHovered()
        )
    {
        std::for_each(_selectedTransforms.begin(), _selectedTransforms.end(), [](auto&& t) { t->deselect(); });
        _selectedTransforms.clear();
        _selectedTransform = nullptr; 
        //deselectAll();
        
    }


    ImGui::End();


    ImGui::Begin("Properties");
	if (auto t = getSelectedTransform())
		t->drawUI();


    ImGui::End();
}
void SceneManager::drawGizmo()
{
    if (!_selectedTransform) return;


    // Make sure to call inside ImGui frame:
    ImGuizmo::BeginFrame();

    // Get viewport size
    ImVec2 windowPos(0, 0);// = ImGui::GetWindowPos();

    auto windowSize = _camera->getWindowSize();

    // Setup ImGuizmo rect
    ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);



    // Choose operation: translate / rotate / scale
    static ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;// | ImGuizmo::ROTATE | ImGuizmo::SCALE;

    glm::mat4 modelMatrix(1);
    modelMatrix = glm::translate(glm::mat4(1.0f), _selectedTransform->getPosition());

    ImGuizmo::Manipulate(
        glm::value_ptr(_camera->getViewMatrix()),
        glm::value_ptr(_camera->getProjectionMatrix()),
        mCurrentGizmoOperation,
        ImGuizmo::LOCAL,
        glm::value_ptr(modelMatrix)
    );

    
    // If modified, write back into your glm::mat4
    if (ImGuizmo::IsUsing()) {
        _selectedTransform->setPosition(glm::vec3(modelMatrix[3]));
    }
}


// LIGHTS 

//void SceneManager::lightUI() {
//    // Shader selection combo box
//    const char* items[] = { "Point", "Direction", "Spot" };
//    static int currentItem = 0;
//
//    ImGui::Begin("Light Controls");
//
//    ImGui::SeparatorText("Add Light");
//    ImGui::Combo("type", &currentItem, items, IM_ARRAYSIZE(items));
//    if (ImGui::Button("Add Light"))
//        addLight(currentItem);
//
//
//    ImGui::SeparatorText("Light List");
//
//    if (ImGui::BeginListBox("listbox 1"))
//    {
//        for (int i{}; i < _lights.size(); i++) //(int n = 0; n < IM_ARRAYSIZE(items); n++)
//        {
//            std::string lightName = _lights[i]->name;
//            const bool is_selected = (selectedLight == i);
//            if (ImGui::Selectable(lightName.c_str(), is_selected)) {
//                //std::cout << "Selected light: " << light.name << std::endl;
//                selectedLight = i;
//            }
//            //if (ImGui::IsItemHovered())
//            //    std::cout << "hovered light: " << light.name << std::endl;
//            //// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
//            //if (is_selected)
//            //    ImGui::SetItemDefaultFocus();
//        }
//        ImGui::EndListBox();
//    }
//
//
//    ImGui::SeparatorText("Light properties");
//    if (selectedLight == -1) { ImGui::End(); return; } // No light selected
//
//    _lights[selectedLight]->drawUI();
//
//    ImGui::End();
//}
const int MAX_LIGHTS = 8; 
int lightCount = 0;
void SceneManager::addLight(LightType lightType)
{
    //static int lightCounter = 0; // Static counter to keep track of light names

    if (lightCount >= MAX_LIGHTS) {
        std::cout << "ERROR: Maximum number of lights reached!" << std::endl;
        return; // Prevent adding more lights if the limit is reached
    }
    std::unique_ptr<Light> newLight;
    std::string lightName = "";
    switch (lightType)
    {
    case LightType::Directional:
        newLight = std::make_unique<DirectionalLight>();
        lightName = "Directional Light" + std::to_string(lightCount);
        break;
    case LightType::Point:
        newLight = std::make_unique<PointLight>();
        lightName = "Point Light" + std::to_string(lightCount);
        break;
    case LightType::Spot:
        newLight = std::make_unique<SpotLight>();
        lightName = "Spot Light" + std::to_string(lightCount);
        break;
    default:
        break;
    }
    Transform* t = new Transform; 
    Entity* en = new Entity;
    en->light = std::move(newLight);
    t->setEntity(en);
    t->name = lightName;
    _transforms.emplace_back(t);


    lightCount++;

}

void SceneManager::addLight(std::unique_ptr<Light> light)
{

    //std::unique_ptr<Light> newLight;
    Transform* t = new Transform;
    t->name = light->name;
    Entity* en = new Entity;
    en->light = std::move(light);
    t->setEntity(en);
    _transforms.emplace_back(t);


    lightCount++;

}

void SceneManager::configShader(Shader& shader)
{
    int counter = 0; 
    for (auto t : _transforms) {
        if (Light* light = t.get()->getEntity()->light.get()) {

            std::string prefix = "_lights[" + std::to_string(counter++) + "].";
            light->configShader(shader, prefix);
        }
    }

    shader.setInt("numLights", counter);
}