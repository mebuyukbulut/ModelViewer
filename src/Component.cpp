#include "Component.h"
#include "Entity.h"
#include <yaml-cpp/yaml.h>

void Component::serialize(YAML::Emitter& out)
{
	Object::serialize(out);
	out << YAML::Key << "componentType" << YAML::Value << ComponentUtils::ToString(type);

}

void Component::deserialize(const YAML::Node& node)
{
	Object::deserialize(node);
	std::string typeStr = node["componentType"].as<std::string>();
	type = ComponentUtils::FromString(typeStr);
}
