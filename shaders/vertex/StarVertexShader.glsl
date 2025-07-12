#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;
out vec3 ViewPos;
out float DistanceToCamera;

void main()
{
    // Calculate world position
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // For a sphere, the normal is just the normalized position
    Normal = normalize(aPos);
    
    // Pass texture coordinates
    TexCoord = aTexCoord;
    
    // Pass view position
    ViewPos = viewPos;
    
    // Calculate distance to camera for glow effect
    DistanceToCamera = length(viewPos - FragPos);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}