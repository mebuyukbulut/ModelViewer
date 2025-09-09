#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "Inspectable.h"

class IColorProvider : public IInspectable {
public:
	virtual glm::vec4 evaluate(float t) const = 0;
};


struct GradientProviderStop {
	float positon; // [0,1]
	glm::vec4 rgba;
};
// TO-DO: https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline
class GradientProvider : public IColorProvider
{
	std::vector<GradientProviderStop> _stops;
	// Maybe we can add a LUT
public:
	void addStop(GradientProviderStop stop) {
		_stops.push_back(stop);
		std::sort(_stops.begin(), _stops.end(), [](GradientProviderStop a, GradientProviderStop b) {return a.positon < b.positon; });
	}

	glm::vec4 evaluate(float t) const {
		if (_stops.empty()) return glm::vec4(1, 1, 1, 1);
		else if (_stops.size() == 1) return _stops[0].rgba;
		else if (t <= 0) return _stops[0].rgba;
		else if (t >= 1) return _stops.back().rgba;

		glm::vec4 color{};

		for (int i{}; i < _stops.size() - 1; i++) {
			float a = _stops[i].positon;
			float b = _stops[i + 1].positon;
			if (a < t && b > t) {
				float localT = (t - a) / (b - a);
				glm::vec4 aColor = _stops[i].rgba;
				glm::vec4 bColor = _stops[i + 1].rgba;
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