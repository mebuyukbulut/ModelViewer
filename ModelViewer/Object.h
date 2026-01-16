#pragma once
#include <string>
#include <cstdint>

class Object{
public:
	uint32_t UUID; 
	std::string name;

	Object() {
		static uint32_t nextUUID = 0;
		UUID = nextUUID++;
		name = "Object_" + std::to_string(UUID);
	}
	virtual ~Object() = default;

	// Her objenin kendini UI'da çizme yeteneği olsun
	virtual void onInspect() {}

	// Her objenin kendini serileştirme (kaydetme) yeteneği olsun
	// virtual YAML::Node serialize() = 0;
};
