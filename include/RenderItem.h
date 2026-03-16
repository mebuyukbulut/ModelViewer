#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include "Model.h"

struct RenderItem
{
	Model* model{};
	glm::mat4 transform{ 1.0f };
	uint32_t entityIndex{}; // for selection pass
};

