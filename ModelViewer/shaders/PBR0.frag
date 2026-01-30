#version 420 core

in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoords;
out vec4 FragColor;

// PBR0.frag veya ilgili shader dosyası
struct Light {
    vec4 position;  // xyz + 1 float padding
    vec4 color;     // rgb + intensity (intensity'yi dördüncü kanal yapabilirsin)
    vec4 params;    // x: attenuation, y: type, z: cutoff, w: padding
    vec4 direction; // xyz + 1 float padding
};

layout (std140, binding = 0) uniform LightBlock {
    Light lights[8];  // Maksimum ışık sayısı (NR_LIGHTS)
    int numLights;    // Aktif ışık sayısı
} ubo_data;


//struct Light {    
//    vec3 position;
//	vec3 color;
//	float intensity;
//    float attenuation;
//    int type; // 0: point, 1: spot, 2: directional
//    vec3 direction; // only for dir and spot
//    float cutoff; // only for spot
//};  
//#define NR_LIGHTS 8  
//uniform Light _lights[NR_LIGHTS];
//uniform int numLights;

layout(binding = 0) uniform sampler2D baseColorTexture;
layout(binding = 1) uniform sampler2D emissiveTexture;
layout(binding = 2) uniform sampler2D metallicTexture;
layout(binding = 3) uniform sampler2D roughnessTexture;
layout(binding = 4) uniform sampler2D aoTexture;


uniform float ambientIntensity;
uniform vec3 ambientColor;

uniform vec3 viewPos;

struct Material{
	vec4 baseColor;
	vec4 emissive;
	float metallic;
	float roughness;
	float reflectance;
	float ao;

    // rgba flags of textures. 0 -> null, 1 -> exits 
    vec4 is_BaseColorTex;
    vec4 is_EmissiveTex;
    vec4 is_MetallicTex;
    vec4 is_RoughnessTex;
    vec4 is_AoTex;
};
uniform Material material;
Material _material;

uniform sampler2D texture_diffuse1;


#define PI 3.1415926535897932384626433832795

float near = 0.1; // near plane    
float far = 100.0; // far plane

float D_GGX(float NoH, float roughness) {
    float a2     = roughness*roughness;
    float NdotH  = NoH;
    float NdotH2 = NdotH*NdotH;
	
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
	
    return nom / denom;


//    float a = NoH * roughness;
//    float k = roughness / (1.0 - NoH * NoH + a * a);
//    return k * k * (1.0 / PI);
}
//// ----------------------------------------------------------------------------
//float GeometrySchlickGGX(float NdotV, float roughness)
//{
//    float r = (roughness + 1.0);
//    float k = (r*r) / 8.0;
//
//    float nom   = NdotV;
//    float denom = NdotV * (1.0 - k) + k;
//
//    return nom / denom;
//}
float V_SmithGGXCorrelated(float NoV, float NoL, float roughness) {
    float a2 = roughness * roughness;
    float GGXV = NoL * sqrt(NoV * NoV * (1.0 - a2) + a2);
    float GGXL = NoV * sqrt(NoL * NoL * (1.0 - a2) + a2);
    return 0.5 / (GGXV + GGXL);

//    float ggx2 = GeometrySchlickGGX(NoV, roughness);
//    float ggx1 = GeometrySchlickGGX(NoL, roughness);
//
//    return ggx1 * ggx2;
}
vec3 F_Schlick(float u, vec3 f0) {
    return f0 + (1 - f0) * pow(1.0 - u, 5.0);
}
float F_Schlick(float u, float f0, float f90) {
    return f0 + (f90 - f0) * pow(1.0 - u, 5.0);
}
float Fd_Burley(float NoV, float NoL, float LoH, float roughness) {
    float f90 = 0.5 + 2.0 * roughness * LoH * LoH;
    float lightScatter = F_Schlick(NoL, 1.0, f90);
    float viewScatter = F_Schlick(NoV, 1.0, f90);
    return lightScatter * viewScatter * (1.0 / PI);
}


float Fd_Lambert() {
    return 1.0 / PI;
}

vec3 CalcPointLight(Light light){
    vec4 baseColor = _material.baseColor;
    float metallic = _material.metallic;
    float perceptualRoughness = _material.roughness;  
    float roughness = clamp(pow(_material.roughness,2), pow(0.01,2), 1) ; 
    float reflectance = _material.reflectance;
    float attRad = light.params.x; 

    vec3 diffuseColor = (1.0 - metallic) * baseColor.rgb;
    vec3 f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + baseColor.rgb * metallic;
  
    vec3 n = normalize(fNormal); // Surface normal vector
    vec3 l = normalize(light.position.xyz - fPos ); // Incident light vector
    vec3 v = normalize(viewPos - fPos); // View/Eye vector
    vec3 h = normalize((v+l)/2); // halfway 
    
    //+ 1e-5; // this create black dots on model maybe we can need this parameter in future but now I remove it 
    float NoV = abs(dot(n,v)) ; //+ 1e-5; // <- this parameter.
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);
    

    float D = D_GGX(NoH, roughness);
    vec3  F = F_Schlick(LoH, f0);
    float V = V_SmithGGXCorrelated(NoV, NoL, roughness);

    // specular BRDF
    vec3 Fr = (D * V) * F;

    // diffuse BRDF
    vec3 Fd = diffuseColor * Fd_Burley(NoV, NoL, LoH, roughness) * (1.0 - F);


    // attenuation
    float distance    = max(length(light.position.xyz - fPos), 0.01);
    float distance2    = distance * distance; // squared distance 
    
    float E = 1.0 / (distance2 * PI); // inverse square law and 1/PI
    float window = 1.0f - (distance2 * distance2 / pow(attRad, 4.0)) ; 
    E *= pow(clamp(window, 0.0, 1.0), 2.0); 

    return (Fd + Fr) * light.color.w * light.color.xyz * NoL * E;
} 

vec3 CalcDirectionalLight(Light light){
    vec4 baseColor = _material.baseColor;
    float metallic = _material.metallic;
    float perceptualRoughness = _material.roughness;  
    float roughness = clamp(pow(_material.roughness,2), pow(0.01,2), 1) ; 
    float reflectance = _material.reflectance;
    //float attRad = light.attenuation;

    vec3 diffuseColor = (1.0 - metallic) * baseColor.rgb;
    vec3 f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + baseColor.rgb * metallic;
  
    vec3 n = normalize(fNormal); // Surface normal vector
    vec3 l = normalize(light.direction.xyz); // Incident light vector
    vec3 v = normalize(viewPos - fPos); // View/Eye vector
    vec3 h = normalize((v+l)/2); // halfway 
    
    //+ 1e-5; // this create black dots on model maybe we can need this parameter in future but now I remove it 
    float NoV = abs(dot(n,v)) ; //+ 1e-5; // <- this parameter.
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);
    

    float D = D_GGX(NoH, roughness);
    vec3  F = F_Schlick(LoH, f0);
    float V = V_SmithGGXCorrelated(NoV, NoL, roughness);

    // specular BRDF
    vec3 Fr = (D * V) * F;

    // diffuse BRDF
    vec3 Fd = diffuseColor * Fd_Burley(NoV, NoL, LoH, roughness) * (1.0 - F);


    // attenuation
//    float distance    = max(length(light.position - fPos), 0.01);
//    float distance2    = distance * distance; // squared distance 
//    
    //float E = 1.0 / (distance2 * PI); // inverse square law and 1/PI
    //float window = 1.0f - (distance2 * distance2 / pow(attRad, 4.0)) ; 
    //E *= pow(clamp(window, 0.0, 1.0), 2.0); 

    return (Fd + Fr) * light.color.w * light.color.xyz * NoL;// * E;
} 


vec3 CalcSpotLight(Light light){
    vec3 lightDir = normalize(fPos - light.position.xyz);
    float theta = dot(lightDir, normalize(light.direction.xyz));
    float phi = cos(radians(light.params.z));

    if(theta < phi) 
      return vec3(0,0,0); 


    vec4 baseColor = _material.baseColor;
    float metallic = _material.metallic;
    float perceptualRoughness = _material.roughness;  
    float roughness = clamp(pow(_material.roughness,2), pow(0.01,2), 1) ; 
    float reflectance = _material.reflectance;
    float attRad = light.params.x;

    vec3 diffuseColor = (1.0 - metallic) * baseColor.rgb;
    vec3 f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + baseColor.rgb * metallic;
  
    vec3 n = normalize(fNormal); // Surface normal vector
    vec3 l = normalize(light.position.xyz - fPos ); // Incident light vector
    vec3 v = normalize(viewPos - fPos); // View/Eye vector
    vec3 h = normalize((v+l)/2); // halfway 
    
    //+ 1e-5; // this create black dots on model maybe we can need this parameter in future but now I remove it 
    float NoV = abs(dot(n,v)) ; //+ 1e-5; // <- this parameter.
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);
    

    float D = D_GGX(NoH, roughness);
    vec3  F = F_Schlick(LoH, f0);
    float V = V_SmithGGXCorrelated(NoV, NoL, roughness);

    // specular BRDF
    vec3 Fr = (D * V) * F;

    // diffuse BRDF
    vec3 Fd = diffuseColor * Fd_Burley(NoV, NoL, LoH, roughness) * (1.0 - F);


    // attenuation
    float distance    = max(length(light.position.xyz - fPos), 0.01);
    float distance2    = distance * distance; // squared distance 
    
    float E = 1.0 / (distance2 * PI); // inverse square law and 1/PI
    float window = 1.0f - (distance2 * distance2 / pow(attRad, 4.0)) ; 
    E *= pow(clamp(window, 0.0, 1.0), 2.0); 



    float angleWindow = 1 - (pow (1 - theta, 4) / pow(1 - phi, 4)); 
    E*= angleWindow; 

    return (Fd + Fr) * light.color.w * light.color.xyz * NoL * E;
} 


float LinearizeDepth(float depth) {
    // Convert depth from NDC to linear depth
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec3 ACESFilm(vec3 x)
{
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14; 
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0); 
}


// texture kullanılmışsa kanala göre atama yapar. 
void setMaterial(){

    if(material.is_BaseColorTex.x == 1)
        _material.baseColor = texture2D(baseColorTexture, fTexCoords);
    else
        _material.baseColor = vec4(1,0,0,1); //material.baseColor;




    if(material.is_EmissiveTex.x == 1)
        _material.emissive = texture(emissiveTexture, fTexCoords);
    else
        _material.emissive = material.emissive;
        



    if(material.is_MetallicTex.x == 1)
        _material.metallic = texture(metallicTexture, fTexCoords).x;
    else if(material.is_MetallicTex.y == 1)
        _material.metallic = texture(metallicTexture, fTexCoords).y;
    else if(material.is_MetallicTex.z == 1)
        _material.metallic = texture(metallicTexture, fTexCoords).z;
    else if(material.is_MetallicTex.a == 1)
        _material.metallic = texture(metallicTexture, fTexCoords).a;
    else
        _material.metallic = material.metallic;




    if(material.is_RoughnessTex.x == 1)
        _material.roughness = texture(roughnessTexture, fTexCoords).x;
    else if(material.is_RoughnessTex.y == 1)
        _material.roughness = texture(roughnessTexture, fTexCoords).y;
    else if(material.is_RoughnessTex.z == 1)
        _material.roughness = texture(roughnessTexture, fTexCoords).z;
    else if(material.is_RoughnessTex.a == 1)
        _material.roughness = texture(roughnessTexture, fTexCoords).a;
    else
        _material.roughness = material.roughness;




    if(material.is_AoTex.x == 1)
        _material.ao = texture(aoTexture, fTexCoords).x;
    else if(material.is_AoTex.y == 1)
        _material.ao = texture(aoTexture, fTexCoords).y;
    else if(material.is_AoTex.z == 1)
        _material.ao = texture(aoTexture, fTexCoords).z;
    else if(material.is_AoTex.a == 1)
        _material.ao = texture(aoTexture, fTexCoords).a;
    else
        _material.ao = material.ao;
}   

void main(){
	setMaterial();

    vec3 pLights = vec3(0.0);
    for(int i = 0; i < ubo_data.numLights; i++){
        float lightType = ubo_data.lights[i].params.y;

        if(lightType == 1) pLights += CalcDirectionalLight(ubo_data.lights[i]);
        if(lightType == 2) pLights += CalcPointLight(ubo_data.lights[i]);
        if(lightType == 3) pLights += CalcSpotLight(ubo_data.lights[i]);
    }

    vec3 result = pLights;// * material.color;
    //vec3 result = (pLights + ambient) * material.color;


    //result = result / (result + vec3(1.0)); // Reinhard tone mapping
    result = ACESFilm(result);
    result = pow(result, vec3(1.0 / 2.2)); // gamma correction

    FragColor = vec4(result,1.0);
    
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //FragColor = vec4(vec3(depth), 1.0);
}	

//FragColor = texture(texture_diffuse1, fTexCoords) * vec4(finalColor, 1);
//FragColor = vec4(finalColor, 1);