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

enum class FXParamKind
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
    FXParamKind type;

    FXValue min;
    FXValue max;
    FXValue defaultVal; 

    template<typename T>
    FXParamDefinition(std::string uniformName, std::string label, std::string tooltip, T min, T max, T defaultVal)
    :uniformName{uniformName}, label{label}, tooltip{tooltip}, min{min}, max{max}, defaultVal{defaultVal}{}
};

struct FXParam{
    const FXParamDefinition* definition = nullptr;
    std::string label{};
    FXValue value; 

    FXParam() = default;

    void update(Shader* shader);

    void setLabel(std::string newLabel){ label = newLabel; }

    std::string getLabel(){ return label; }
    std::string getTooltip(){ return definition->tooltip; }


};

struct FXInstanceDefinition{
    std::string builtinID{};    // builtin::fx::grayscale 
    std::string label{}; 
    std::string vertexPath{};   // ./assets/fx/...
    std::string fragmentPath{}; // ./assets/fx/...
    std::vector<FXParam> parameters{};

    FXInstanceDefinition() = default;
    FXInstanceDefinition(std::string builtinID, std::string vertexPath, std::string fragmentPath, std::vector<FXParam> parameters)
        :builtinID{builtinID}, vertexPath{vertexPath}, fragmentPath{fragmentPath}, parameters{parameters}{}
};

class FXInstance : public IInspectable{
    bool enabled{};
    float opacity{1.0f};
    std::string builtinID{};
    std::string label{}; 
    std::vector<FXParam> parameters{}; 

    Shader* getShader();
    // onInspect(); 

    FXInstance() = default;
    FXInstance(FXInstanceDefinition definition);

    void update();

    void onInspect();
    
};

class FXRegistry{
    static std::vector<FXInstanceDefinition> FXDefinitionStack;
    std::vector<FXInstance> FXStack{};

    void init(); // init all shaders 

    void addShader();
    
    // void onInspect();
    
};

std::vector<FXInstanceDefinition> FXRegistry::FXDefinitionStack{
    FXInstanceDefinition{Builtin::FX::Grayscale, "fullscreen_tris.vert", "grayscale.frag", 
        {}},

};



// {Builtin::FX::Grayscale,     "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/grayscale.frag"},
// {Builtin::FX::PassThrough,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/passthrough.frag"},
// {Builtin::FX::Invert,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/invert.frag"},
// {Builtin::FX::Sepia,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/sepia.frag"},
// {Builtin::FX::Vignette,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/vignette.frag"},
// {Builtin::FX::GammaCorrection,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/gamma_correction.frag"},
// {Builtin::FX::Posterize,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/posterize.frag"},
// {Builtin::FX::Pixelate,   "../assets/shaders/postfx/fullscreen_tris.vert", "../assets/shaders/postfx/pixelate.frag"},