#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

struct GradientStop {
	float positon; // [0,1]
	glm::vec4 rgba; 
};

class Gradient
{
	std::vector<GradientStop> _stops;
	// Maybe we can add a LUT
public:
	void addStop(GradientStop stop) {
		_stops.push_back(stop); 
		std::sort(_stops.begin(), _stops.end(), [](GradientStop a, GradientStop b) {return a.positon < b.positon; });
	}

	glm::vec4 evaluate(float t) {
		if (_stops.empty()) return glm::vec4(1, 1, 1, 1);
		else if (_stops.size() == 1) return _stops[0].rgba;
		else if (t <= 0) return _stops[0].rgba;
		else if (t >= 1) return _stops.back().rgba;

		glm::vec4 color{};

		for (int i{}; i < _stops.size()-1 ; i++) {
			float a = _stops[i].positon;
			float b = _stops[i+1].positon;
			if (a < t && b > t) {
				float localT = (t - a) / (b - a);
				glm::vec4 aColor = _stops[i].rgba;
				glm::vec4 bColor = _stops[i+1].rgba;
				return aColor * (1 - localT) + bColor * localT;
			}
		}

		return color;
	}
};

