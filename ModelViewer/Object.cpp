#include "Object.h"
#include "yaml-cpp/yaml.h"

void Object::serialize(YAML::Emitter& out)
{
	out << YAML::Key << "UUID" << YAML::Value << UUID;
	out << YAML::Key << "name" << YAML::Value << name;
}

void Object::deserialize(const YAML::Node& node)
{
}
