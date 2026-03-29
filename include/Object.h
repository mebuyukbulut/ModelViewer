#pragma once
#include <string>
#include <cstdint>
#include "IInspectable.h"

namespace YAML {
	class Emitter;
	class Node;
}

class Object : public IInspectable{
public:
	uint64_t UUID; 
	std::string name;

	Object();
	virtual ~Object() = default;

	// Her objenin kendini UI'da çizme yeteneği olsun
	virtual void onInspect() override {}

	// Her objenin kendini serileştirme (kaydetme) yeteneği olsun
	virtual void serialize(YAML::Emitter& out);
	virtual void deserialize(const YAML::Node& node);
};
