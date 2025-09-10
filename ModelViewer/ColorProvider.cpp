#include "ColorProvider.h"
#include <imgui.h>
#include <string>
#include <iostream>


// https://github.com/epezent/implot
void GradientProvider::drawUI()
{
    GradientEditor(_stops);
    ImGui::ColorEdit4("color edit", &_stops[_selectedStop].color[0]);
    
	////ImGui::SeparatorText("Gradient");
	//auto a = ImGui::GetWindowDrawList();

	//ImVec4 color(1, 1, 1, 1);
	//a->AddRectFilled(a->GetClipRectMin(), ImVec2(200, 200), ImGui::GetColorU32(color));
	////a->AddRectFilledMultiColor(a->GetClipRectMin())
}
bool GradientProvider::GradientEditor(std::vector<GradientStop>& stops, ImVec2 size) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p0 = ImGui::GetCursorScreenPos();
    p0.x += 5;
    size.x =ImGui::GetWindowWidth()-25;
    ImVec2 p1 = ImVec2(p0.x + size.x, p0.y + size.y);

    // Gradient bar background
    draw_list->AddRectFilled(p0, p1, IM_COL32(0, 0, 0, 255)); // 50 50 50 255

    // Draw each segment
    for (size_t i = 0; i + 1 < _stops.size(); i++) {
        auto a = _stops[i].color;
        auto b = _stops[i + 1].color;
        ImVec4 c0(a.r, a.g, a.b, a.a);
        ImVec4 c1(b.r, b.g, b.b, b.a);
        ImU32 col0 = ImGui::ColorConvertFloat4ToU32(c0);//(_stops[i].color);
        ImU32 col1 = ImGui::ColorConvertFloat4ToU32(c1);//(_stops[i + 1].color);
        float x0 = p0.x + _stops[i].position * size.x;
        float x1 = p0.x + _stops[i + 1].position * size.x;
        draw_list->AddRectFilledMultiColor(
            ImVec2(x0, p0.y), ImVec2(x1, p1.y),
            col0, col1, col1, col0
        );


        if (i == 0 && _stops[i].position != 0) {
            draw_list->AddRectFilledMultiColor(
                ImVec2(p0.x, p0.y), ImVec2(x0, p1.y),
                col0, col0, col0, col0
            );
        }
        if (i+2 == _stops.size() && _stops[i+1].position != 1) {
            draw_list->AddRectFilledMultiColor(
                ImVec2(x1, p0.y), ImVec2(p1.x, p1.y),
                col1, col1, col1, col1
            );

        }
    }



    ImGui::InvisibleButton("gradientRect", size);

    if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left)) {
        float newPos = (ImGui::GetIO().MousePos.x - p0.x) / size.x;
        float clampedNewPos = std::clamp(newPos, 0.0f, 1.0f);

        addStop(clampedNewPos);
        //std::cout << "new stop added" << std::endl;
    }




    // Draw stop handles
    bool changed = false;
    for (size_t i = 0; i < _stops.size(); i++) {
        float x = p0.x + _stops[i].position * size.x;
        ImVec2 pos(x, p1.y);
        ImVec2 handleMin(pos.x - 5, p1.y);
        ImVec2 handleMax(pos.x + 5, p1.y + 10);

        ImGui::SetCursorScreenPos(handleMin);
        ImGui::InvisibleButton(("stop" + std::to_string(i)).c_str(), ImVec2(10, 15));

        if (ImGui::IsItemActive() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            _selectedStop = i;
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            float newPos = (ImGui::GetIO().MousePos.x - p0.x) / size.x;
            float safeOffset = 0.02;
            if (i == 0) {
                _stops[i].position = std::clamp(newPos, 0.0f, _stops[1].position - safeOffset);
            }
            else if (i + 1 == _stops.size()) {
                _stops[i].position = std::clamp(newPos, _stops[i-1].position+ safeOffset, 1.0f);
            }
            else {
                _stops[i].position = std::clamp(newPos, _stops[i - 1].position+ safeOffset, _stops[i + 1].position- safeOffset);

            }


            _selectedStop = i;
            changed = true;
        }

        draw_list->AddTriangleFilled(
            ImVec2(pos.x, p1.y - 3),
            ImVec2(pos.x - 5, p1.y + 3),
            ImVec2(pos.x + 5, p1.y + 3),
            IM_COL32(79, 79, 79, 255)
        );
        draw_list->AddRectFilled(
            ImVec2(pos.x - 5, p1.y + 3),
            ImVec2(pos.x + 5, p1.y + 15),
            IM_COL32(79, 79, 79, 255)
        );

        draw_list->AddRectFilled(
            ImVec2(pos.x - 3, p1.y + 6),
            ImVec2(pos.x + 3, p1.y + 13),
            IM_COL32(0,0,0, 255)
        );
        ImVec4 stopColor(_stops[i].color.r, _stops[i].color.g, _stops[i].color.b, _stops[i].color.a);
        draw_list->AddRectFilled(
            ImVec2(pos.x - 3, p1.y + 6),
            ImVec2(pos.x + 3, p1.y + 13),
            ImGui::ColorConvertFloat4ToU32(stopColor)
        );
    }
    ImGui::SetCursorScreenPos(p0); // we reset cursor to gradient editor begin position
    size.y += 20;
    ImGui::Dummy(size); // reserve space
    return changed;
}


void ConstantColorProvider::drawUI()
{
	//ImGui::SeparatorText("Constant");
	ImGui::ColorEdit4("color", &rgba[0]);
}

void ColorOverLifeProvider::drawUI()
{
	//ImGui::SeparatorText("Color lifetime");
	ImGui::ColorEdit4("start color", &start[0]);
	ImGui::ColorEdit4("end color", &end[0]);

}