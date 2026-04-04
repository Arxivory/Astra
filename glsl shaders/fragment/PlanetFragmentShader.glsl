#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN; // Received from vertex shader

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    // 1. Sample Normal Map and transform to [-1, 1] range
    vec3 normal = texture(normalMap, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0); 
    
    // 2. Transform normal from Tangent Space to World Space
    vec3 N = normalize(TBN * normal);

    // 3. Lighting Logic (Standard Lambert)
    vec3 color = texture(diffuseMap, TexCoords).rgb;
    vec3 L = normalize(lightPos - FragPos);
    float diff = max(dot(L, N), 0.0);

    // 4. Atmospheric Rim (Fresnel)
    vec3 V = normalize(viewPos - FragPos);
    float rim = pow(1.0 - max(dot(V, N), 0.0), 4.0);
    vec3 rimColor = vec3(0.4, 0.6, 1.0) * diff; // Blue tint for atmosphere

    vec3 ambient = 0.05 * color;
    vec3 finalColor = ambient + (diff * color) + (rim * 0.2);

    FragColor = vec4(finalColor, 1.0);
}