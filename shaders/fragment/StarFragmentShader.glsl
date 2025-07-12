#version 330 core

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec3 ViewPos;
in float DistanceToCamera;

uniform sampler2D starTexture;

// Star properties (you can pass these as uniforms from your C++ code)
uniform float luminosity;    // How bright the star is
uniform float temperature;   // Star temperature (affects color)
uniform float size;          // Star size (for glow calculation)

out vec4 FragColor;

// Convert temperature to color (simplified blackbody radiation)
vec3 temperatureToColor(float temp) {
    // Normalize temperature (3000K to 10000K range)
    float normalizedTemp = clamp((temp - 3000.0) / 7000.0, 0.0, 1.0);
    
    vec3 color;
    if (normalizedTemp < 0.5) {
        // Red to yellow (cooler stars)
        float t = normalizedTemp * 2.0;
        color = mix(vec3(1.0, 0.3, 0.1), vec3(1.0, 1.0, 0.3), t);
    } else {
        // Yellow to blue-white (hotter stars)
        float t = (normalizedTemp - 0.5) * 2.0;
        color = mix(vec3(1.0, 1.0, 0.3), vec3(0.7, 0.8, 1.0), t);
    }
    
    return color;
}

void main()
{
    // Sample the base texture
    vec4 texColor = texture(starTexture, TexCoord);
    
    // Get color based on temperature
    vec3 starColor = temperatureToColor(temperature);
    
    // Calculate center-to-edge falloff for glow effect
    vec2 center = vec2(0.5, 0.5);
    float distFromCenter = length(TexCoord - center);
    
    // Create a bright core with soft edges
    float coreIntensity = 1.0 - smoothstep(0.0, 0.3, distFromCenter);
    float glowIntensity = 1.0 - smoothstep(0.0, 0.8, distFromCenter);
    
    // Calculate view-dependent glow (brighter when looking directly at it)
    vec3 viewDir = normalize(ViewPos - FragPos);
    float viewDotNormal = max(dot(viewDir, Normal), 0.0);
    float rimGlow = pow(1.0 - viewDotNormal, 2.0);
    
    // Combine different glow components
    float totalGlow = coreIntensity * 2.0 + glowIntensity * 1.5 + rimGlow * 0.8;
    
    // Apply luminosity scaling
    float brightnessScale = luminosity / 1000.0; // Assuming luminosity is in some reasonable range
    brightnessScale = clamp(brightnessScale, 0.1, 10.0);
    
    // Distance-based intensity (stars should be bright even at distance)
    float distanceAttenuation = 1.0 + (size * 0.1); // Larger stars maintain brightness better
    
    // Combine texture with star color and effects
    vec3 finalColor = texColor.rgb * starColor;
    finalColor *= totalGlow * brightnessScale * distanceAttenuation;
    
    // Add some bloom/overexposure effect for very bright areas
    float brightness = dot(finalColor, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0) {
        finalColor += (brightness - 1.0) * starColor * 0.5;
    }
    
    // Ensure the star is always visible and bright
    finalColor = max(finalColor, starColor * 0.3);
    
    FragColor = vec4(finalColor, 1.0);
}