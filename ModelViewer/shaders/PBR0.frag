#version 330 core

in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoords;
out vec4 FragColor;

struct PointLight {    
    vec3 position;
	vec3 color;
	float intensity;
    float attenuation;
    int type; // 0: point, 1: dir, 2: spot
    vec3 direction; // only for dir and spot
    float cutoff; // only for spot
};  
#define NR_POINT_LIGHTS 8  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int numLights;

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
};
uniform Material material;

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

vec3 CalcPointLight(PointLight light)
{
    vec4 baseColor = material.baseColor;
    float metallic = material.metallic;
    float perceptualRoughness = material.roughness;
    float reflectance = material.reflectance;

    vec3 diffuseColor = (1.0 - metallic) * baseColor.rgb;
    vec3 f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + baseColor.rgb * metallic;
    float roughness = perceptualRoughness; 
    roughness = clamp(roughness, 0.01,1); 
    roughness *= roughness; 
    //float roughness = perceptualRoughness * perceptualRoughness;

    vec3 n = normalize(fNormal); // Surface normal vector
    vec3 l = normalize(light.position - fPos ); // Incident light vector
    vec3 v = normalize(viewPos - fPos); // View/Eye vector
    vec3 h = normalize((v+l)/2); // halfway 
    
    //+ 1e-5; // this create black dots on model maybe we can need this parameter in future but now I remove it 
    float NoV = abs(dot(n,v)) ; //+ 1e-5; // <- this parameter.
    //float NoV = dot(n,v);
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
    //vec3 Fd = diffuseColor * Fd_Lambert();








//    vec3 lightDir = normalize(light.position - fragPos);
//
//
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    vec3 diffuse  = (1.0f -  material.specularIntensity) * diff * light.intensity * light.color; 
//
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//    vec3 specular =  material.specularIntensity * spec * light.intensity * light.color; 
//


//	float constant = 1.0f;
//	float linear   = 0.09f;
//	float quadratic = 0.032f;

    // attenuation
    float distance    = max(length(light.position - fPos), 0.01);
    //float distance2    = max(dot(light.position, fPos), 0.0001); // squared distance 
    float distance2    = distance * distance; // squared distance 
    float attRad = light.attenuation; 

    float E = 1.0 / (distance2 * PI);
    float window = 1.0f - (distance2 * distance2 / pow(attRad, 4.0)) ; 
    E *= pow(clamp(window, 0.0, 1.0), 2.0); 

    

    //return (Fr + Fd);
   // return (Fr + Fd) * attenuation * light.intensity * light.color;

        // add to outgoing radiance Lo
    return (Fd + Fr) * light.intensity * light.color * NoL * E;

} 

float LinearizeDepth(float depth) 
{
    // Convert depth from NDC to linear depth
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	

    vec3 pLights = vec3(0.0);
    for(int i = 0; i < numLights; i++){
        pLights += CalcPointLight(pointLights[i]);
    }

    vec3 result = pLights;// * material.color;
    //vec3 result = (pLights + ambient) * material.color;

    //// HDR ? 
    //result = result / (result + vec3(1.0));
    //result = pow(result, vec3(1.0/2.2)); 

    FragColor = vec4(result, 1.0);    
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //FragColor = vec4(vec3(depth), 1.0);
}	

//FragColor = texture(texture_diffuse1, fTexCoords) * vec4(finalColor, 1);
//FragColor = vec4(finalColor, 1);