#include "Object.h"
#include <random>
#include "yaml-cpp/yaml.h"

#include <iostream>

class UUIDGenerator {
public:
    static uint64_t generate() {
        static std::random_device rd;
        static std::mt19937_64 engine(rd());
        static std::uniform_int_distribution<uint64_t> dist;

        return dist(engine);
    }
};

Object::Object() {
    UUID = UUIDGenerator::generate();
    name = "Object_" + std::to_string(UUID);
    std::cout << "UUID: " << UUID << std::endl;
}

void Object::serialize(YAML::Emitter& out)
{
	out << YAML::Key << "UUID" << YAML::Value << UUID;
	out << YAML::Key << "name" << YAML::Value << name;
}

void Object::deserialize(const YAML::Node& node)
{
    UUID = node["UUID"].as<uint32_t>();
	name = node["name"].as<std::string>();
}
