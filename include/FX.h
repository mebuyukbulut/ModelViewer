#pragma once

#include <string>
#include <variant>
#include <vector>
#include <glm/glm.hpp>
#include "Builtin.h"
#include "IInspectable.h"

class Shader;

using FXValue = std::variant<
    float,
    int,
    glm::vec2,
    glm::vec3,
    glm::vec4,
    bool
>;

enum class FXParamType
{
    Float,
    Int,
    Bool,
    Vec2,
    Vec3,
    Vec4,
};

struct FXParamDefinition{
    std::string uniformName{};
    std::string label{};
    std::string tooltip{};
    FXParamType type;

    FXValue min;
    FXValue max;
    FXValue defaultVal; 

    template<typename T>
    FXParamDefinition(std::string uniformName, std::string label, std::string tooltip, FXParamType type, T min, T max, T defaultVal)
    :uniformName{uniformName}, label{label}, tooltip{tooltip}, type{type}, min{min}, max{max}, defaultVal{defaultVal}{}
};

struct FXParam : public IInspectable{
    const FXParamDefinition* definition = nullptr;
    bool dirty{true};
    FXValue value; 

    FXParam() = default;
    FXParam(const FXParamDefinition* definition);

    void update(Shader* shader);

    template<typename T> 
    void setValue(T newValue){ value = newValue; dirty = true; }


    std::string getUniformName(){ return definition->uniformName; }
    std::string getLabel(){ return definition->label; }
    std::string getTooltip(){ return definition->tooltip; }

    FXParamType getType(){ return definition->type; }

    FXValue getValue(){ return value; }
    FXValue getMin(){ return definition->min; }
    FXValue getMax(){ return definition->max; }
    FXValue getDefaultValue(){ return definition->defaultVal; }

    void onInspect() override;

};

struct FXInstanceDefinition{
    std::string builtinID{};    // builtin::fx::grayscale 
    std::string label{}; 
    std::string tooltip{};
    std::string vertexPath{};   // ./assets/fx/...
    std::string fragmentPath{}; // ./assets/fx/...
    std::vector<const FXParamDefinition*> parameters{};

    FXInstanceDefinition() = default;
    FXInstanceDefinition(
        std::string builtinID, 
        std::string label,
        std::string tooltip,
        std::string vertexPath, std::string fragmentPath, 
        std::vector<const FXParamDefinition*> parameters)
        :
        builtinID{builtinID}, 
        label{label},
        tooltip{tooltip},
        vertexPath{vertexPath}, fragmentPath{fragmentPath}, 
        parameters{parameters}{}
};

class FXInstance : public IInspectable{
    bool enabled{true};
    float opacity{1.0f};
    std::string builtinID{};
    std::string label{}; 
    std::vector<FXParam> parameters{}; 

    // onInspect(); 
public:
    FXInstance() = default;
    FXInstance(const FXInstanceDefinition& definition);

    Shader* getShader();

    void update();

    void onInspect() override;
    
};

class FXRegistry : public IInspectable{
    static std::vector<FXInstanceDefinition> FXInstanceDefinitionStack;
    std::vector<FXInstance> FXStack{};

    void addInstance(int definitionIndex);
    void addInstance(std::string builtinID);

    std::vector<std::string> getDefinitionList();
public:
    void init(); // init all shaders 

    void onInspect() override;
    
};

namespace Builtin
{
    namespace FX
    {
        namespace Params{
            inline const FXParamDefinition MyFloat {"u_float", "my float:", "This is a float", FXParamType::Float,  0.0f, 1.0f, 0.5f};


        }
    }
}


// {Builtin::FX::Grayscale,     "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/grayscale.frag"},
// {Builtin::FX::PassThrough,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/passthrough.frag"},
// {Builtin::FX::Invert,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/invert.frag"},

// {Builtin::FX::Sepia,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/sepia.frag"},
// {Builtin::FX::Vignette,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/vignette.frag"},
// {Builtin::FX::GammaCorrection,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/gamma_correction.frag"},

// {Builtin::FX::Posterize,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/posterize.frag"},
// {Builtin::FX::Pixelate,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/pixelate.frag"},