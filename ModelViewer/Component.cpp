#include "Component.h"
#include "Entity.h"
#include <yaml-cpp/yaml.h>

void Component::serialize(YAML::Emitter& out) const
{
	Object::serialize(out);

}

void Component::deserialize(const YAML::Node& node)
{
}
