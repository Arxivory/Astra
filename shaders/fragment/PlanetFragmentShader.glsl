#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN; 

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 geomNormal = normalize(TBN[2]);

    float poleWeight = abs(geomNormal.y);
    poleWeight = smoothstep(0.85, 0.95, poleWeight); 

    vec3 nm = texture(normalMap, TexCoords).rgb;
    nm = normalize(nm * 2.0 - 1.0);
    
    vec3 worldNormalMap = normalize(TBN * nm);

    vec3 N = normalize(mix(worldNormalMap, geomNormal, poleWeight));

    vec3 color = texture(diffuseMap, TexCoords).rgb;
    vec3 L = normalize(lightPos - FragPos);
    vec3 V = normalize(viewPos - FragPos);

    float diff = max(dot(L, N), 0.0);

    float rimFactor = pow(1.0 - max(dot(V, geomNormal), 0.0), 4.0);
    
    vec3 rimColor = vec3(0.4, 0.6, 1.0); 
    vec3 atmosphericRim = rimColor * rimFactor * diff;

    vec3 ambient = 0.03 * color;
    vec3 diffuse = diff * color;
    
    vec3 finalColor = ambient + diffuse + atmosphericRim;

    finalColor = pow(finalColor, vec3(1.0/1.2));

    FragColor = vec4(finalColor, 1.0);
}