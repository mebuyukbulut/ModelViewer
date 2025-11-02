#version 330 core
in vec3 worldPos;
out vec4 FragColor;

uniform vec3 gridColor = vec3(0.55, 0.55, 0.55);
uniform float scale = 1.0;

void main() {
    vec3 finalColor = gridColor;
    float lineThickness = 1;
    vec2 coord = worldPos.xz / scale;


    
    // çizgi oluşturmak için fractional kısım:
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    
    float line = min(grid.x, grid.y);
    float intensity = 1.0 - smoothstep(0.0, lineThickness, line);


    if (abs(coord.x) < lineThickness/100.0f) {
        finalColor = vec3(0.0, 0.0, 0.8); // mavi
        intensity = 1.0 - smoothstep(0.0, lineThickness*5, line);
        intensity = 1.0;
    }else if(abs(coord.y) < lineThickness/100.0f){
        finalColor = vec3(0.8, 0.0, 0.0); // kırmızı
        intensity = 1.0 - smoothstep(0.0, lineThickness*5, line);
        intensity = 1.0;

    }

    FragColor = vec4(finalColor, intensity);
    //FragColor = vec4(0,1,0, 1.0);
}
