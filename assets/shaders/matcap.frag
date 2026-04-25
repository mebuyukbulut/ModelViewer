#version 420 core

in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoords;
out vec4 FragColor;

uniform sampler2D matcapTexture;

void main()
{
    vec3 n = normalize(fNormal);
    vec2 muv = n.xy * 0.5 + 0.5;

    FragColor = texture(matcapTexture, vec2(muv.x, 1.0-muv.y));

    
}