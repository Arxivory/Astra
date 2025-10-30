#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float bumpStrength;

void main() {
    vec3 color = texture(diffuseMap, TexCoords).rgb;

    // Normal mapping
    vec3 normal = texture(normalMap, TexCoords).rgb;
    normal = normalize(mix(vec3(0.0, 0.0, 1.0), normal, bumpStrength));
    normal = normalize(TBN * normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);

    float brightness = 1.2;

    vec3 diffuse = diff * color;
    vec3 ambient = 0.1 * color;

    FragColor = vec4(brightness * (ambient + diffuse), 1.0);
}
