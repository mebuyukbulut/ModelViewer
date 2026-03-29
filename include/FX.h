#pragma once

#include <string>
#include <variant>
#include <vector>
#include "Builtin.h"

class Shader;

enum class FXParamType
{
    Float,
    Int,
    Vec2,
    Vec3,
    Vec4,
    Bool
};

class FXParam{
    std::string uniformName{};
    std::string label{};
    std::variant<FXParamType> value; 
    std::variant<FXParamType> min, max, defaultVal; // requires for UI 
};

class FXInstanceDefinition{
    std::string builtinID{};    // builtin::fx::grayscale 
    std::string label{}; 
    std::string vertexPath{};   // ./assets/fx/...
    std::string fragmentPath{}; // ./assets/fx/...
    std::vector<FXParam> parameters{};

public:
    FXInstanceDefinition() = default;
    FXInstanceDefinition(std::string builtinID, std::string vertexPath, std::string fragmentPath, std::vector<FXParam> parameters)
        :builtinID{builtinID}, vertexPath{vertexPath}, fragmentPath{fragmentPath}, parameters{parameters}{}
};

class FXInstance{
    bool enabled;
    float opacity{1.0f};
    std::string builtinID{};
    std::string label{}; 
    std::vector<FXParam> parameters{}; 

    Shader* getShader();
    // onInspect(); 

    FXInstance() = default;
    FXInstance(FXInstanceDefinition definition);


};

class FXRegistry{
    static std::vector<FXInstanceDefinition> FXDefinitionStack;
    std::vector<FXInstance> FXStack{};

    void init(); // init all shaders 

    void addShader();
    
    // void onInspect();
    
};

std::vector<FXInstanceDefinition> FXRegistry::FXDefinitionStack{
    {Builtin::FX::Grayscale, "fullscreen_tris.vert", "grayscale.frag", 
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