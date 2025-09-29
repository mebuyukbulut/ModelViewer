#include "SceneManager.h"
#include <imgui.h>


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


        //fmt::format()
        ImGui::TreeNodeEx("1bc", flag2); ImGui::TreePop();
        if (ImGui::TreeNodeEx("2bc")) { 
            ImGui::TreeNodeEx("0bc", flag2); ImGui::TreePop();
            ImGui::TreeNodeEx("1bc", flag2); ImGui::TreePop();
            ImGui::TreePop();
        }

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
