#version 430 core

in vec2 fTexCoords;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D frameTex;

void main(){
    float d = distance(fTexCoords, vec2(0.5));
    
    vec3 color = texture(frameTex, fTexCoords).rgb;
    color *= smoothstep(0.8, 0.4, d);
    FragColor = vec4(color, 1.0f);
}