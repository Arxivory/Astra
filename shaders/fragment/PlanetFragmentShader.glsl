#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D diffuseMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 color = texture(diffuseMap, TexCoords).rgb;

    vec3 N = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, N), 0.0);

    float brightness = 1.2;
    vec3 diffuse = diff * color;
    vec3 ambient = 0.1 * color;

    FragColor = vec4(brightness * (ambient + diffuse), 1.0);
}