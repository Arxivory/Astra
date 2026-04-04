#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D skyboxTexture;

void main() {
    vec3 envColor = texture(skyboxTexture, TexCoords).rgb;
    
    FragColor = vec4(envColor * 1.5, 1.0); 
}