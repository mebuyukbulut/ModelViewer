#pragma once
#include <string>
#include <cstdint>

namespace YAML {
	class Emitter;
	class Node;
}

class Object{
public:
	uint32_t UUID; 
	std::string name;

	Object();
	virtual ~Object() = default;

	// Her objenin kendini UI'da çizme yeteneği olsun
	virtual void onInspect() {}

	// Her objenin kendini serileştirme (kaydetme) yeteneği olsun
	virtual void serialize(YAML::Emitter& out);
	virtual void deserialize(const YAML::Node& node);
};
