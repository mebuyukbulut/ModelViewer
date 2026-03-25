#pragma once

namespace Builtin
{
    namespace Material{
        inline constexpr const char* DefaultMaterial = "builtin::materials::defaultMaterial";

    }
    namespace Model
    {
        inline constexpr const char* Cube        = "builtin::models::cube";
        inline constexpr const char* Cone        = "builtin::models::cone";
        inline constexpr const char* Cylinder    = "builtin::models::cylinder";
        inline constexpr const char* Sphere      = "builtin::models::sphere";
        inline constexpr const char* Plane       = "builtin::models::plane";
        inline constexpr const char* Torus       = "builtin::models::torus";

        inline constexpr const char* BgPlane     = "builtin::models::bgPlane";
        inline constexpr const char* GridPlane   = "builtin::models::gridPlane";

        inline constexpr const char* LightArrow  = "builtin::models::lightArrow";
        inline constexpr const char* LightCone   = "builtin::models::lightCone";
        inline constexpr const char* LightSphere = "builtin::models::lightSphere";

        inline constexpr const char* All[] = {
            Cube,       
            Cone,       
            Cylinder,   
            Sphere,
            Plane,      
            Torus,      
            BgPlane,    
            GridPlane,  
            LightArrow, 
            LightCone,  
            LightSphere,
        };
    }

    namespace Shader
    {
        inline constexpr const char* PBR        = "builtin::shaders::pbr";
        inline constexpr const char* Matcap     = "builtin::shaders::matcap";
        inline constexpr const char* Wireframe  = "builtin::shaders::wireframe";
        inline constexpr const char* Grid       = "builtin::shaders::grid";
        inline constexpr const char* Background = "builtin::shaders::bg";
        inline constexpr const char* Selection  = "builtin::shaders::selection";
        inline constexpr const char* Shadow     = "builtin::shaders::shadow";
    }
}