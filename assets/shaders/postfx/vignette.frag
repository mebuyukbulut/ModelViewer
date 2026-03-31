#version 430 core

in vec2 fTexCoords;
out vec4 FragColor;

uniform float u_distance = 0.1;
uniform float u_feather = 0.4;
uniform float u_amount = 0.0;

layout(binding = 0) uniform sampler2D frameTex;

float random(vec2 uv) {
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(){
    float d = distance(fTexCoords, vec2(0.5));
    
    vec3 color = texture(frameTex, fTexCoords).rgb;
    color *= max(smoothstep( min(u_distance+u_feather, 0.99), u_distance, d), 
                 random(fTexCoords)*(1.0f - u_amount) );
    
    FragColor = vec4(color, 1.0f);
}