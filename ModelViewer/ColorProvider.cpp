#include "ColorProvider.h"
#include <imgui.h>

void GradientProvider::drawUI()
{
	//ImGui::SeparatorText("Gradient");
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