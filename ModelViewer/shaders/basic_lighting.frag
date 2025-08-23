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
uniform vec3 objectColor;
uniform float specularIntensity;


uniform sampler2D texture_diffuse1;


#define _PI 3.1415926535897932384626433832795

float near = 0.1; // near plane    
float far = 100.0; // far plane

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	float constant = 1.0f;
	float linear   = 0.09f;
	float quadratic = 0.032f;


    vec3 lightDir = normalize(light.position - fragPos);


    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse  = (1.0f - specularIntensity) * diff * light.intensity * light.color; 

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularIntensity * spec * light.intensity * light.color; 

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    

    return (diffuse + specular)*attenuation;
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
	vec3 ambient = ambientIntensity * ambientColor; 

    //vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
    vec3 pLights = vec3(0.0);
    for(int i = 0; i < numLights; i++){
        pLights += CalcPointLight(pointLights[i], surfaceNormal, fPos, viewDir);
    }

    vec3 result = (pLights + ambient) * objectColor;

    FragColor = vec4(result, 1.0);    
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //FragColor = vec4(vec3(depth), 1.0);
}	

//FragColor = texture(texture_diffuse1, fTexCoords) * vec4(finalColor, 1);
//FragColor = vec4(finalColor, 1);