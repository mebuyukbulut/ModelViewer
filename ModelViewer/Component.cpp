#include "Component.h"
#include "Entity.h"
#include <yaml-cpp/yaml.h>

void Component::serialize(YAML::Emitter& out)
{
	Object::serialize(out);

}

void Component::deserialize(const YAML::Node& node)
{
}
