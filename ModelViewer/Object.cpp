#include "Object.h"
#include <random>
#include "yaml-cpp/yaml.h"

#include <iostream>
#include <sstream>

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
    // UUID'yi 0x ön ekiyle ve büyük harf hex olarak biçimlendiriyoruz
    std::stringstream ss;
    ss << "0x" << std::hex << std::uppercase << UUID;

    out << YAML::Key << "UUID" << YAML::Value << ss.str();
	//out << YAML::Key << "UUID" << YAML::Value << UUID;
	out << YAML::Key << "name" << YAML::Value << name;
}

void Object::deserialize(const YAML::Node& node)
{
    
    std::string hexID = node["UUID"].as<std::string>();
    // std::stoull hem normal sayıları hem de 0x ile başlayan hexleri anlar
    UUID = std::stoull(hexID, nullptr, 0);
    
    //UUID = node["UUID"].as<uint64_t>();
	name = node["name"].as<std::string>();
}
