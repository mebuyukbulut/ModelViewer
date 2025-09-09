#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "Inspectable.h"
#include <imgui.h>

class IColorProvider : public IInspectable {
public:
	virtual glm::vec4 evaluate(float t) const = 0;
};


struct GradientStop {
	float position; // [0,1]
	glm::vec4 color;
};
// TO-DO: https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline
class GradientProvider : public IColorProvider
{
	int _selectedStop{};
	std::vector<GradientStop> _stops{ GradientStop{0,{0,1,0,1}}, GradientStop{1,{0,1,0,0}} };
	// Maybe we can add a LUT

	bool GradientEditor(std::vector<GradientStop>& stops, ImVec2 size = ImVec2(200, 40));

public:
	void addStop(GradientStop stop) {
		_stops.push_back(stop);
		std::sort(_stops.begin(), _stops.end(), [](GradientStop a, GradientStop b) {return a.position < b.position; });
	}

	glm::vec4 evaluate(float t) const {
		if (_stops.empty()) return glm::vec4(1, 1, 1, 1);
		else if (_stops.size() == 1) return _stops[0].color;
		else if (t <= 0) return _stops[0].color;
		else if (t >= 1) return _stops.back().color;

		glm::vec4 color{};

		for (int i{}; i < _stops.size() - 1; i++) {
			float a = _stops[i].position;
			float b = _stops[i + 1].position;
			if (a < t && b > t) {
				float localT = (t - a) / (b - a);
				glm::vec4 aColor = _stops[i].color;
				glm::vec4 bColor = _stops[i + 1].color;
				return aColor * (1 - localT) + bColor * localT;
			}
		}

		return color;
	}

	// Inherited via IColorProvider
	void drawUI() override;
};

class ConstantColorProvider : public IColorProvider {
public:
	glm::vec4 rgba{1,1,1,1};
	glm::vec4 evaluate(float t) const {
		return rgba;
	}

	// Inherited via IColorProvider
	void drawUI() override;
};

class ColorOverLifeProvider : public IColorProvider {
public:
	glm::vec4 start{ 1,1,1,1 };
	glm::vec4 end{ 1,1,1,0 };
	glm::vec4 evaluate(float t) const {
		return glm::mix(start, end, t);
	}

	// Inherited via IColorProvider
	void drawUI() override;
};