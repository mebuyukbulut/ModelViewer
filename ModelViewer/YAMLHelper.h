#pragma once
#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace YAML {
    // 1. Yazma (Serialize) İçin: operator<< Overload
    inline Emitter& operator<<(Emitter& out, const glm::vec3& v) {
        out << Flow; // [x, y, z] şeklinde tek satırda yazar
        out << BeginSeq << v.x << v.y << v.z << EndSeq;
        return out;
    }
    inline Emitter& operator<<(Emitter& out, const glm::quat& v) {
        out << Flow; // [w, x, y, z] şeklinde tek satırda yazar
        out << BeginSeq << v.w << v.x << v.y << v.z << EndSeq;
        return out;
    }

    // 2. Okuma (Deserialize) İçin: YAML::convert Özelleştirmesi
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& v) {
            Node node;
            node.push_back(v.x);
            node.push_back(v.y);
            node.push_back(v.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& v) {
            if (!node.IsSequence() || node.size() != 3) return false;
            v.x = node[0].as<float>();
            v.y = node[1].as<float>();
            v.z = node[2].as<float>();
            return true;
        }
    };
    template<>
    struct convert<glm::quat> {
        static Node encode(const glm::quat& v) {
            Node node;
            node.push_back(v.w);
            node.push_back(v.x);
            node.push_back(v.y);
            node.push_back(v.z);
            return node;
        }

        static bool decode(const Node& node, glm::quat& v) {
            if (!node.IsSequence() || node.size() != 4) return false;
            v.w = node[0].as<float>();
            v.x = node[1].as<float>();
            v.y = node[2].as<float>();
            v.z = node[3].as<float>();
            return true;
        }
    };
}





//
//
//namespace YAML {
//    template<>
//    struct convert<glm::vec3> {
//        // glm::vec3 → YAML (C++ objesini dosyaya yazarken)
//        static Node encode(const glm::vec3& rhs) {
//            Node node;
//            //node.push_back(std::vector<float>{ rhs.x, rhs.y, rhs.z });
//            node.SetStyle(YAML::EmitterStyle::Flow);
//            node.push_back(rhs.x);
//            node.push_back(rhs.y);
//            node.push_back(rhs.z);
//            return node;
//        }
//
//        // YAML → glm::vec3 (dosyadan okurken)
//        static bool decode(const Node& node, glm::vec3& rhs) {
//            if (!node.IsSequence() || node.size() != 3)
//                return false;
//            rhs.x = node[0].as<float>();
//            rhs.y = node[1].as<float>();
//            rhs.z = node[2].as<float>();
//            return true;
//        }
//    };
//}





//namespace YAML {
//    template<>
//    struct convert<glm::vec3> {
//        // glm::vec3 → YAML (C++ objesini dosyaya yazarken)
//        static Node encode(const glm::vec3& rhs) {
//            Node node;
//            //node.push_back(std::vector<float>{ rhs.x, rhs.y, rhs.z });
//            node.SetStyle(YAML::EmitterStyle::Flow);
//            node.push_back(rhs.x);
//            node.push_back(rhs.y);
//            node.push_back(rhs.z);
//            return node;
//        }
//
//        // YAML → glm::vec3 (dosyadan okurken)
//        static bool decode(const Node& node, glm::vec3& rhs) {
//            if (!node.IsSequence() || node.size() != 3)
//                return false;
//            rhs.x = node[0].as<float>();
//            rhs.y = node[1].as<float>();
//            rhs.z = node[2].as<float>();
//            return true;
//        }
//    };
//}