#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D diffuseMap;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform float shininess;
uniform float specularStrength;
uniform float ambientStrength;

void main() {
    vec3 color = texture(diffuseMap, TexCoords).rgb;

    // sensible fallbacks if the application hasn't set the uniforms
    vec3 lc = (length(lightColor) > 0.0) ? lightColor : vec3(1.0);
    float sh = (shininess > 0.0) ? shininess : 32.0;
    float sp = (specularStrength > 0.0) ? specularStrength : 0.5;
    float am = (ambientStrength > 0.0) ? ambientStrength : 0.1;

    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - FragPos);
    vec3 V = normalize(viewPos - FragPos);

    // diffuse (Lambert)
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * color * lc;

    // specular (Phong)
    vec3 R = reflect(-L, N);
    float spec = 0.0;
    if (diff > 0.0) {
        spec = pow(max(dot(R, V), 0.0), sh) * sp;
    }
    vec3 specular = spec * lc;

    // ambient
    vec3 ambient = am * color;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}