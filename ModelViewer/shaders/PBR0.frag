#version 330 core

in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoords;
out vec4 FragColor;

struct PointLight {    
    vec3 position;
	vec3 color;
	float intensity;
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
    vec3 color;
    float specularIntensity;
};
uniform Material material;

uniform sampler2D texture_diffuse1;


#define PI 3.1415926535897932384626433832795

float near = 0.1; // near plane    
float far = 100.0; // far plane

float D_GGX(float NoH, float roughness) {
    float a = NoH * roughness;
    float k = roughness / (1.0 - NoH * NoH + a * a);
    return k * k * (1.0 / PI);
}
float V_SmithGGXCorrelated(float NoV, float NoL, float roughness) {
    float a2 = roughness * roughness;
    float GGXV = NoL * sqrt(NoV * NoV * (1.0 - a2) + a2);
    float GGXL = NoV * sqrt(NoL * NoL * (1.0 - a2) + a2);
    return 0.5 / (GGXV + GGXL);
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


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float perceptualRoughness = material.specularIntensity;
    //float perceptualRoughness = 0.5; 
    vec3 f0 = vec3(0,0,0);
    vec3 diffuseColor = material.color;


    vec3 n = normalize(fNormal); // Surface normal vector
    vec3 l = normalize(fPos - light.position); // Incident light vector
    //vec3 l = normalize(light.position - fPos); // Incident light vector
    vec3 v = normalize(viewPos - fPos); // View/Eye vector
    vec3 h = normalize((n+v)/2); // halfway 

    float NoV = abs(dot(n,v)) + 1e-5;
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);
    
    // perceptually linear roughness to roughness (see parameterization)
    float roughness = perceptualRoughness * perceptualRoughness;

    float D = D_GGX(NoH, roughness);
    vec3  F = F_Schlick(LoH, f0);
    float V = V_SmithGGXCorrelated(NoV, NoL, roughness);

    // specular BRDF
    vec3 Fr = (D * V) * F;

    // diffuse BRDF
    vec3 Fd = diffuseColor * Fd_Burley(NoV, NoL, LoH, roughness);








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
	float constant = 1.0f;
	float linear   = 0.09f;
	float quadratic = 0.032f;

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    

    return (Fr + Fd)*attenuation;
} 

float LinearizeDepth(float depth) 
{
    // Convert depth from NDC to linear depth
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{

	// Calculate normals
	vec3 surfaceNormal = normalize(fNormal);
	//vec3 lightDir = normalize(lightPos - fPos);
	vec3 viewDir = normalize(viewPos - fPos);
	
	// ambient 
	//vec3 ambient = ambientIntensity * ambientColor; 

    //vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
    vec3 pLights = vec3(0.0);
    for(int i = 0; i < numLights; i++){
        pLights += CalcPointLight(pointLights[i], surfaceNormal, fPos, viewDir);
    }

    vec3 result = pLights * material.color;
    //vec3 result = (pLights + ambient) * material.color;

    FragColor = vec4(result, 1.0);    
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //FragColor = vec4(vec3(depth), 1.0);
}	

//FragColor = texture(texture_diffuse1, fTexCoords) * vec4(finalColor, 1);
//FragColor = vec4(finalColor, 1);