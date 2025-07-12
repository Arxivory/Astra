//#define STB_IMAGE_IMPLEMENTATION
//
//#include <stb/stb_image.h>
//#include <map>
//
//#define GLM_FORCE_RADIANS
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <imgui.h>
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
//
//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <algorithm>
//#include <sstream>
//#include <string>
//#include <fstream>
//
//#ifndef M_PI
//#define M_PI 3.14159265358979323846
//#endif
//
//using namespace std;
//using namespace ImGui;
//
//const unsigned int SCR_WIDTH = 1280;
//const unsigned int SCR_HEIGHT = 720;
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//
//float yaw = -90.0f;
//float pitch = 0.0f;
//float fov = 45.0f;
//
//glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 50.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//bool mousePressed = false;
//
//const char* galaxyVertexShaderSource = R"(
//#version 330 core
//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aColor;
//
//out vec3 ourColor;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//void main() {
//    gl_Position = projection * view * model * vec4(aPos, 1.0);
//    gl_PointSize = 0.9;
//    ourColor = aColor;
//}
//)";
//
//const char* galaxyFragmentShaderSource = R"(
//#version 330 core
//in vec3 ourColor;
//out vec4 FragColor;
//
//void main() {
//    FragColor = vec4(ourColor, 0.7);
//}
//)";
//
//
//// Create a simple shader for stars
//const char* starVertexShaderSource = R"(
//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in float aBrightness;
//
//uniform mat4 projection;
//uniform mat4 view;
//
//out float brightness;
//
//void main() {
//    gl_Position = projection * view * vec4(aPos, 1.0);
//    brightness = aBrightness;
//    gl_PointSize = 2.5;
//}
//)";
//
//const char* starFragmentShaderSource = R"(
//#version 330 core
//out vec4 FragColor;
//in float brightness;
//
//void main() {
//    FragColor = vec4(1.0, 1.0, 1.0, brightness);
//}
//)";
//
//const char* sunFragmentShaderSource = R"(
//#version 330 core
//out vec4 FragColor;
//
//in vec2 TexCoord;
//in vec3 FragPos;
//in vec3 Normal;
//
//uniform sampler2D texture1;
//uniform float time;
//
//void main() {
//    // Base sun texture
//    vec3 sunColor = texture(texture1, TexCoord).rgb;
//
//    // Add pulsating glow effect
//    float glowIntensity = 1.0 + 0.2 * sin(time * 0.5);
//    vec3 glowColor = vec3(1.0, 0.7, 0.3) * glowIntensity; // Warm yellow-orange glow
//
//    // Mix the texture with the glow effect
//    vec3 result = mix(sunColor, glowColor, 0.3);
//
//    // Add corona effect (stronger at the edges)
//    vec3 viewDir = normalize(-FragPos);
//    float rim = 1.0 - max(dot(viewDir, normalize(Normal)), 0.0);
//    rim = pow(rim, 2.0) * 1.5;
//
//    // Add the rim light for corona effect
//    result += vec3(1.0, 0.6, 0.2) * rim;
//
//    // Make sure the sun is always bright
//    result = clamp(result, vec3(0.8), vec3(1.0));
//
//    FragColor = vec4(result, 1.0);
//}
//)";
//
//const char* vertexShaderSource = R"(
//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoord;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//out vec2 TexCoord;
//out vec3 FragPos;
//out vec3 Normal;
//
//void main()
//{
//    vec4 worldPos = model * vec4(aPos, 1.0);
//    FragPos = worldPos.xyz;
//    Normal = mat3(transpose(inverse(model))) * aPos; // correct normal transform
//    gl_Position = projection * view * worldPos;
//    TexCoord = aTexCoord;
//}
//
//)";
//
//const char* fragmentShaderSource = R"(
//#version 330 core
//out vec4 FragColor;
//
//in vec2 TexCoord;
//in vec3 FragPos;
//in vec3 Normal;
//
//uniform vec3 lightPos;
//uniform vec3 viewPos;
//uniform sampler2D texture1;
//
//void main()
//{
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(lightPos - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//
//    vec3 diffuse = diff * vec3(1.0); // white light
//    vec3 ambient = vec3(0.1);
//
//    vec3 objectColor = texture(texture1, TexCoord).rgb;
//    vec3 result = (ambient + diffuse) * objectColor;
//    FragColor = vec4(result, 1.0);
//}
//)";
//
//const char* bgVertexShaderSource = R"(
//#version 330 core
//layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec2 aTexCoord;
//out vec2 TexCoord;
//void main() {
//    gl_Position = vec4(aPos.xy, 0.0, 1.0);
//    TexCoord = aTexCoord;
//}
//)";
//
//const char* bgFragmentShaderSource = R"(
//#version 330 core
//out vec4 FragColor;
//in vec2 TexCoord;
//uniform sampler2D background;
//void main() {
//    FragColor = texture(background, TexCoord);
//}
//)";
//
//const char* flareVertexShaderSource = R"(
//#version 330 core
//layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec2 aTexCoord;
//
//uniform mat4 model;
//out vec2 TexCoord;
//
//void main() {
//    gl_Position = model * vec4(aPos, 0.0, 1.0);
//    TexCoord = aTexCoord;
//}
//)";
//
//const char* flareFragmentShaderSource = R"(
//#version 330 core
//out vec4 FragColor;
//in vec2 TexCoord;
//
//uniform sampler2D flareTexture;
//uniform vec3 flareColor;
//uniform float flareAlpha;
//
//void main() {
//    vec4 texColor = texture(flareTexture, TexCoord);
//    FragColor = vec4(flareColor * texColor.rgb, texColor.a * flareAlpha);
//}
//)";
//
//const char* shipVertexShaderSource = R"(
//#version 330 core
//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aNormal;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//out vec3 Normal;
//out vec3 FragPos;
//
//void main() {
//    FragPos = vec3(model * vec4(aPos, 1.0));
//    Normal = mat3(transpose(inverse(model))) * aNormal;
//    gl_Position = projection * view * vec4(FragPos, 1.0);
//}
//)";
//
//const char* shipFragmentShaderSource = R"(
//#version 330 core
//in vec3 Normal;
//in vec3 FragPos;
//out vec4 FragColor;
//
//uniform vec3 lightPos;
//uniform vec3 viewPos;
//
//void main() {
//    vec3 lightColor = vec3(1.0);
//    vec3 objectColor = vec3(0.7, 0.5, 0.3);
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(lightPos - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = diff * lightColor;
//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//    vec3 specular = 0.5 * spec * lightColor;
//    vec3 result = (diffuse + specular) * objectColor;
//    FragColor = vec4(result, 1.0);
//}
//)";
//
//bool showOrbitLines = true;  // Default to showing orbit lines
//std::vector<std::vector<float>> orbitLineVertices;  // Store orbit vertices for each planet
//GLuint orbitLinesVAO, orbitLinesVBO;
//
//float logSliderValue = 0.0f; // internal value from 0.0 to 1.0
//float cameraSpeedSlider = 10.0f;
//float cameraSpeed = 10.0f;
//float timeFactor = 0.0f;
//
//void ShowPlanetStatisticsWithGraph(float &timefactor) {
//
//    ImGui::SetNextWindowPos(ImVec2(600, 100), ImGuiCond_FirstUseEver);
//    ImGui::Begin("Planetary Statistics");
//
//    float gravity[] = { 3.7f, 8.87f, 9.81f, 3.71f, 24.79f };
//
//    // === Table Section ===
//    static const char* columns[] = {
//        "Planet", "Mass (kg)", "Radius (km)", "Gravity (m/s²)", "Orbital Period (days)"
//    };
//
//    if (ImGui::BeginTable("PlanetStats", IM_ARRAYSIZE(columns), ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
//        for (int column = 0; column < IM_ARRAYSIZE(columns); column++) {
//            ImGui::TableSetupColumn(columns[column], ImGuiTableColumnFlags_WidthFixed);
//        }
//        ImGui::TableHeadersRow();
//
//        const char* planets[] = { "Mercury", "Venus", "Earth", "Mars", "Jupiter" };
//        float mass[] = { 3.30e23f, 4.87e24f, 5.97e24f, 6.42e23f, 1.90e27f };
//        float radius[] = { 2439.7f, 6051.8f, 6371.0f, 3389.5f, 69911.0f };
//        float period[] = { 88.0f, 224.7f, 365.25f, 687.0f, 4331.0f };
//
//        for (int row = 0; row < 5; ++row) {
//            ImGui::TableNextRow();
//            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", planets[row]);
//            ImGui::TableSetColumnIndex(1); ImGui::Text("%.2e", mass[row]);
//            ImGui::TableSetColumnIndex(2); ImGui::Text("%.1f", radius[row]);
//            ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f", gravity[row]);
//            ImGui::TableSetColumnIndex(4); ImGui::Text("%.2f", period[row]);
//        }
//
//        ImGui::EndTable();
//    }
//
//    // === Statistical Analysis Section ===
//    ImGui::Separator();
//    ImGui::Text("Statistical Analysis");
//
//    float meanGravity = 10.0f;
//    float stdDevGravity = 8.0f;
//    float regressionSlope = 0.0023f;
//    float regressionIntercept = 1.4f;
//    float correlationRadiusGravity = 0.86f;
//    float pValue = 0.003f;
//
//    ImGui::Text("Mean Gravity: %.2f m/s²", meanGravity);
//    ImGui::Text("Standard Deviation (Gravity): %.2f", stdDevGravity);
//    ImGui::Text("Correlation (Radius vs Gravity): %.2f", correlationRadiusGravity);
//    ImGui::Text("Linear Regression (Gravity = m * Radius + b):");
//    ImGui::BulletText("m = %.4f", regressionSlope);
//    ImGui::BulletText("b = %.2f", regressionIntercept);
//    ImGui::Text("P-value: %.4f", pValue);
//
//    // === Gravity Plot ===
//    ImGui::Separator();
//    ImGui::Text("Gravity Plot (per planet)");
//
//    ImGui::PlotLines("Gravity (m/s²)", gravity, IM_ARRAYSIZE(gravity), 0, "Planets", 0.0f, 30.0f, ImVec2(0, 150));
//    ImGui::Separator();
//
//    // Slider from 0 to 1 (you see 1.0 on the left, 0.000001 on the right)
//    ImGui::SliderFloat("Log Slider", &logSliderValue, 0.000001f, 1.0f, "%.6f", ImGuiSliderFlags_Logarithmic);
//
//    timefactor = logSliderValue;
//
//    ImGui::SliderFloat("Camera Speed", &cameraSpeedSlider, 0.0f, 10000.0f, "%.5f", ImGuiSliderFlags_Logarithmic);
//
//    cameraSpeed = cameraSpeedSlider;
//
//    ImGui::End();
//}
//
//
//void ShowPlanetDashboard() {
//    ImGui::Begin("Planet Dashboard");
//
//    ImGui::Text("Selected Planet: %s", "Earth");
//    ImGui::Separator();
//
//    if (ImGui::CollapsingHeader("Display Options", ImGuiTreeNodeFlags_DefaultOpen)) {
//        ImGui::Checkbox("Show Orbit Lines", &showOrbitLines);
//    }
//
//    if (ImGui::CollapsingHeader("Basic Information", ImGuiTreeNodeFlags_DefaultOpen)) {
//        ImGui::Text("Mass: %.2e kg", 5.97e24);
//        ImGui::Text("Radius: %.2f km", 6371.0f);
//        ImGui::Text("Average Distance from Sun: %.2f million km", 149.6f);
//        ImGui::Text("Orbital Period: %.2f days", 365.25f);
//    }
//
//    if (ImGui::CollapsingHeader("Orbital Parameters", ImGuiTreeNodeFlags_DefaultOpen)) {
//        ImGui::Text("Eccentricity: %.4f", 0.0167f);
//        ImGui::Text("Semi-major Axis: %.2f AU", 1.0f);
//        ImGui::Text("Inclination: %.2f degrees", 0.0f);
//        ImGui::Text("Longitude of Ascending Node: %.2f degrees", 348.74f);
//        ImGui::Text("Argument of Periapsis: %.2f degrees", 114.21f);
//    }
//
//    if (ImGui::CollapsingHeader("Surface Data", ImGuiTreeNodeFlags_DefaultOpen)) {
//        ImGui::Text("Gravity: %.2f m/s^2", 9.81f);
//        ImGui::Text("Escape Velocity: %.2f km/s", 11.19f);
//        ImGui::Text("Average Temperature: %.2f °C", 15.0f);
//        ImGui::Text("Atmosphere: %s", "78% N2, 21% O2");
//    }
//
//    if (ImGui::CollapsingHeader("Live Data (Simulation)", ImGuiTreeNodeFlags_DefaultOpen)) {
//        ImGui::Text("Position (x, y, z): (%.1f, %.1f, %.1f)", 120.0f, 80.0f, 300.0f);
//        ImGui::Text("Velocity: %.2f km/s", 29.78f);
//        ImGui::Text("Distance from Earth: %.2f million km", 0.0f);
//        ImGui::Text("Angular Velocity: %.4f deg/day", 0.9856f);
//    }
//
//    ImGui::End();
//}
//
//struct Particle {
//    glm::vec3 position;
//    glm::vec3 color;
//};
//
//std::vector<Particle> loadGalaxyParticlesWithColor(const char* imagePath, int maxParticles) {
//    int width, height, channels;
//    stbi_set_flip_vertically_on_load(true);
//    unsigned char* data = stbi_load(imagePath, &width, &height, &channels, 0);
//    if (!data) {
//        std::cerr << "Failed to load galaxy image" << std::endl;
//        exit(1);
//    }
//
//    std::vector<Particle> particles;
//    float dispersion = 0.02f;
//    for (int y = 0; y < height && particles.size() < maxParticles; ++y) {
//        for (int x = 0; x < width && particles.size() < maxParticles; ++x) {
//            int idx = (y * width + x) * channels;
//            unsigned char r = data[idx];
//            unsigned char g = data[idx + 1];
//            unsigned char b = data[idx + 2];
//
//            // Calculate brightness (optional, for threshold)
//            float brightness = (r + g + b) / (3.0f * 255.0f);
//            if (brightness > 0.2f) {
//                float nx = ((float)x / width - 0.5f) * 2.0f;
//                float ny = ((float)y / height - 0.5f) * 2.0f;
//                float offsetX = ((float)rand() / RAND_MAX) * 4.0f * dispersion - dispersion;
//                float offsetY = ((float)rand() / RAND_MAX) * 5.0f * dispersion - dispersion;
//                float offsetZ = ((float)rand() / RAND_MAX) * 3.0f * dispersion - dispersion;
//
//                glm::vec3 pos = glm::vec3(nx + offsetX, ny + offsetY, offsetZ);
//
//                // Normalize color to [0,1]
//                glm::vec3 color = glm::vec3(r, g, b) / 255.0f;
//
//                particles.push_back({ pos, color });
//            }
//        }
//    }
//
//    stbi_image_free(data);
//    return particles;
//}
//
////setup galaxy
//
//GLuint galaxyVAO, galaxyVBO;
//auto particles = loadGalaxyParticlesWithColor("textures/galaxy.jpg", 600000);
//
//void setupGalaxy() {
//
//    std::vector<float> data;
//    for (auto& p : particles) {
//        data.push_back(p.position.x);
//        data.push_back(p.position.y);
//        data.push_back(p.position.z);
//
//        data.push_back(p.color.r);
//        data.push_back(p.color.g);
//        data.push_back(p.color.b);
//    }
//
//    glGenVertexArrays(1, &galaxyVAO);
//    glGenBuffers(1, &galaxyVBO);
//
//    glBindVertexArray(galaxyVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, galaxyVBO);
//    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
//
//    // position
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // color
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    glBindVertexArray(0);
//
//}
//
//struct LensFlare {
//    glm::vec2 position;
//    float size;
//    glm::vec3 color;
//    float alpha;
//};
//
//GLuint generateLensFlareTexture(int width = 256, int height = 256) {
//    unsigned char* data = new unsigned char[width * height * 4];
//
//    // Create radial gradient (brightest in center, fading to edges)
//    float centerX = width / 2.0f;
//    float centerY = height / 2.0f;
//    // Use the smaller dimension to ensure a perfect circle even if width != height
//    float radius = std::min(centerX, centerY);
//
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            int index = (y * width + x) * 4;
//
//            // Calculate distance from center
//            float dx = (x - centerX);
//            float dy = (y - centerY);
//            float distance = sqrt(dx * dx + dy * dy);
//            float normalizedDist = distance / radius;  // Normalize to radius for perfect circle
//
//            // Set alpha to 0 outside the circle
//            if (normalizedDist > 1.0f) {
//                data[index + 0] = 255;
//                data[index + 1] = 255;
//                data[index + 2] = 255;
//                data[index + 3] = 0;
//                continue;
//            }
//
//            // Create smooth falloff (sharper at edges)
//            float intensity = 1.0f - pow(normalizedDist, 2.0f);
//            intensity = std::max(0.0f, intensity);
//
//            // Add some subtle rings
//            intensity += 0.1f * sin(normalizedDist * 15.0f) * intensity;
//            intensity = std::min(1.0f, std::max(0.0f, intensity));
//
//            // Set all RGBA components
//            data[index + 0] = static_cast<unsigned char>(255);
//            data[index + 1] = static_cast<unsigned char>(255);
//            data[index + 2] = static_cast<unsigned char>(255);
//            data[index + 3] = static_cast<unsigned char>(intensity * 255);
//        }
//    }
//
//    // Create OpenGL texture
//    GLuint texture;
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    // Set texture parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    delete[] data;
//    return texture;
//}
//
//void generateRingMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, float innerRadius = 1.2f, float outerRadius = 2.5f, int segments = 128) {
//    vertices.clear();
//    indices.clear();
//
//    for (int i = 0; i <= segments; ++i) {
//        float angle = 2.0f * M_PI * float(i) / float(segments);
//        float xInner = innerRadius * cos(angle);
//        float zInner = innerRadius * sin(angle);
//        float xOuter = outerRadius * cos(angle);
//        float zOuter = outerRadius * sin(angle);
//
//        // Inner vertex (closer to Saturn)
//        vertices.push_back(xInner);
//        vertices.push_back(0.0f);
//        vertices.push_back(zInner);
//        vertices.push_back(0.0f);  // u
//        vertices.push_back(0.0f);  // v
//
//        // Outer vertex
//        vertices.push_back(xOuter);
//        vertices.push_back(0.0f);
//        vertices.push_back(zOuter);
//        vertices.push_back(1.0f);  // u
//        vertices.push_back(1.0f);  // v
//    }
//
//    for (int i = 0; i < segments * 2; i += 2) {
//        indices.push_back(i);
//        indices.push_back(i + 1);
//        indices.push_back(i + 2);
//
//        indices.push_back(i + 1);
//        indices.push_back(i + 3);
//        indices.push_back(i + 2);
//    }
//}
//
//
//void generateSphereMesh(vector<float>& vertices, vector<unsigned int>& indices, int sectorCount = 64, int stackCount = 64) {
//    const float PI = 3.14159265359f;
//
//    // Clear the vectors in case they contain data
//    vertices.clear();
//    indices.clear();
//
//    float x, y, z, xy;
//    float nx, ny, nz, lengthInv = 1.0f;
//    float s, t;
//
//    float sectorStep = 2 * PI / sectorCount;
//    float stackStep = PI / stackCount;
//    float sectorAngle, stackAngle;
//
//    for (int i = 0; i <= stackCount; ++i) {
//        stackAngle = PI / 2 - i * stackStep;   // starting from pi/2 to -pi/2
//        xy = cosf(stackAngle);                 // r * cos(u)
//        z = sinf(stackAngle);                  // r * sin(u)
//
//
//        for (int j = 0; j <= sectorCount; ++j) {
//            sectorAngle = j * sectorStep;      // starting from 0 to 2pi
//
//
//            x = xy * cosf(sectorAngle);        // r * cos(u) * cos(v)
//            y = xy * sinf(sectorAngle);        // r * cos(u) * sin(v)
//
//
//            nx = x * lengthInv;
//            ny = y * lengthInv;
//            nz = z * lengthInv;
//
//
//            s = (float)j / sectorCount;
//            t = 1.0f - (float)i / stackCount;  // Flip the texture vertically
//
//
//            vertices.push_back(x);
//            vertices.push_back(z);
//            vertices.push_back(y);
//            vertices.push_back(s);
//            vertices.push_back(t);
//        }
//    }
//
//    // Generate indices
//    int k1, k2;
//    for (int i = 0; i < stackCount; ++i) {
//        k1 = i * (sectorCount + 1);     // beginning of current stack
//        k2 = k1 + sectorCount + 1;      // beginning of next stack
//
//        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
//            // 2 triangles per sector excluding first and last stacks
//            if (i != 0) {
//                indices.push_back(k1);
//                indices.push_back(k2);
//                indices.push_back(k1 + 1);   // k1---k2---k1+1
//            }
//
//            if (i != (stackCount - 1)) {
//                indices.push_back(k1 + 1);
//                indices.push_back(k2);
//                indices.push_back(k2 + 1);   // k1+1---k2---k2+1
//            }
//        }
//    }
//}
//
//
//GLuint loadTexture(const char* path) {
//    int width, height, nrChannels;
//    stbi_set_flip_vertically_on_load(true);
//    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
//    if (!data) {
//        std::cerr << "Failed to load texture: " << path << std::endl;
//        return 0;
//    }
//    GLuint texture;
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, width, height, 0,
//        nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
//    glGenerateMipmap(GL_TEXTURE_2D);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    stbi_image_free(data);
//    return texture;
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//void processInput(GLFWwindow* window) {
//    float speed = cameraSpeed * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += speed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= speed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
//}
//
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
//    if (ImGui::GetIO().WantCaptureMouse) return;
//
//    if (button == GLFW_MOUSE_BUTTON_LEFT) {
//        if (action == GLFW_PRESS) {
//            mousePressed = true;
//            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//            // Save the current mouse position to avoid jumps
//            double xpos, ypos;
//            glfwGetCursorPos(window, &xpos, &ypos);
//            lastX = xpos;
//            lastY = ypos;
//            firstMouse = true;  // Reset this to avoid camera jump
//        }
//        else if (action == GLFW_RELEASE) {
//            mousePressed = false;
//            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//        }
//    }
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
//    if (!mousePressed) return;  // Only process mouse movement when pressed
//
//    if (firstMouse) {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos;
//    lastX = xpos;
//    lastY = ypos;
//    float sensitivity = 0.1f;
//    xoffset *= sensitivity;
//    yoffset *= sensitivity;
//    yaw += xoffset;
//    pitch += yoffset;
//    if (pitch > 89.0f) pitch = 89.0f;
//    if (pitch < -89.0f) pitch = -89.0f;
//    glm::vec3 front;
//    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    front.y = sin(glm::radians(pitch));
//    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//    cameraFront = glm::normalize(front);
//}
//
//struct Planet {
//    std::string name;
//    float distance;
//    float size;
//    float orbitSpeed;
//    float rotationSpeed;
//    float initialAngle;
//    GLuint textureID;
//};
//
//vector<float> vertices;
//vector<unsigned int> indices;
//
//GLuint starVAO, starVBO;
//vector<float> starVertices;
//
//void generateStars(int count = 570) {
//    starVertices.clear(); // Clear any existing stars
//    for (int i = 0; i < count; ++i) {
//        // Generate stars in a much larger sphere (distance from -1000 to 1000)
//        float x = ((rand() % 4000) - 2000);
//        float y = ((rand() % 4000) - 2000);
//        float z = ((rand() % 4000) - 2000);
//
//        // Normalize the direction to place stars on a sphere
//        float length = sqrt(x * x + y * y + z * z);
//
//        // Make sure we don't divide by zero
//        if (length < 0.0001f) length = 0.0001f;
//
//        // Place stars at a distance between 900 and 1000 units
//        float distance = 1300.0f + (rand() % 100);
//
//        starVertices.push_back((x / length) * distance);
//        starVertices.push_back((y / length) * distance);
//        starVertices.push_back((z / length) * distance);
//
//        float brightness = 0.5f + ((rand() % 50) / 100.0f); // 0.5 to 1.0
//        starVertices.push_back(brightness);
//    }
//}
//
//GLuint flareVAO, flareVBO, flareEBO;
//GLuint flareShaderProgram;
//GLuint flareTexture;
//
//void setupLensFlare() {
//    GLuint flareVertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(flareVertexShader, 1, &flareVertexShaderSource, NULL);
//    glCompileShader(flareVertexShader);
//
//    GLuint flareFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(flareFragmentShader, 1, &flareFragmentShaderSource, NULL);
//    glCompileShader(flareFragmentShader);
//
//    flareShaderProgram = glCreateProgram();
//    glAttachShader(flareShaderProgram, flareVertexShader);
//    glAttachShader(flareShaderProgram, flareFragmentShader);
//    glLinkProgram(flareShaderProgram);
//
//    glDeleteShader(flareVertexShader);
//    glDeleteShader(flareFragmentShader);
//
//    // Create quad for rendering flares
//    float flareQuadVertices[] = {
//        // positions   // texCoords
//        -1.0f,  1.0f,  0.0f, 1.0f,
//        -1.0f, -1.0f,  0.0f, 0.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,
//         1.0f,  1.0f,  1.0f, 1.0f
//    };
//
//    unsigned int flareIndices[] = {
//        0, 1, 2,
//        0, 2, 3
//    };
//
//    glGenVertexArrays(1, &flareVAO);
//    glGenBuffers(1, &flareVBO);
//    glGenBuffers(1, &flareEBO);
//
//    glBindVertexArray(flareVAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, flareVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(flareQuadVertices), flareQuadVertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, flareEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(flareIndices), flareIndices, GL_STATIC_DRAW);
//
//    // Position attribute
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // TexCoord attribute
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    // Generate flare texture
//    flareTexture = generateLensFlareTexture();
//}
//
//void renderLensFlare(const LensFlare& flare) {
//    glUseProgram(flareShaderProgram);
//
//    // Create model matrix for positioning and scaling
//    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::translate(model, glm::vec3(flare.position, 0.0f));
//    model = glm::scale(model, glm::vec3(flare.size));
//
//    // Set uniforms
//    glUniformMatrix4fv(glGetUniformLocation(flareShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//    glUniform3fv(glGetUniformLocation(flareShaderProgram, "flareColor"), 1, glm::value_ptr(flare.color));
//    glUniform1f(glGetUniformLocation(flareShaderProgram, "flareAlpha"), flare.alpha);
//
//    // Bind texture
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, flareTexture);
//    glUniform1i(glGetUniformLocation(flareShaderProgram, "flareTexture"), 0);
//
//    // Draw flare
//    glBindVertexArray(flareVAO);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//}
//
//// Function to calculate lens flare positions and properties
//std::vector<LensFlare> calculateLensFlares(const glm::vec3& sunWorldPos,
//    const glm::mat4& view,
//    const glm::mat4& projection) {
//    std::vector<LensFlare> flares;
//
//    // Convert sun position to clip space
//    glm::vec4 sunClipPos = projection * view * glm::vec4(sunWorldPos, 1.0);
//
//    // Perspective division to get NDC (Normalized Device Coordinates)
//    if (sunClipPos.w != 0) {
//        sunClipPos.x /= sunClipPos.w;
//        sunClipPos.y /= sunClipPos.w;
//        sunClipPos.z /= sunClipPos.w;
//    }
//
//    // Only show flares if sun is in front of camera
//    if (sunClipPos.z > 1.0)
//        return flares;
//
//    // Sun position in NDC space (-1 to 1)
//    glm::vec2 sunNDC = glm::vec2(sunClipPos.x, sunClipPos.y);
//
//    // Skip flares if sun is offscreen (with a bit of margin)
//    if (sunNDC.x < -1.2 || sunNDC.x > 1.2 || sunNDC.y < -1.2 || sunNDC.y > 1.2)
//        return flares;
//
//    // Vector from sun to screen center
//    glm::vec2 toCenter = -sunNDC;
//    float distToCenter = glm::length(toCenter);
//    glm::vec2 direction = toCenter / distToCenter;
//
//    // Primary flare (at sun position)
//    LensFlare primaryFlare;
//    primaryFlare.position = sunNDC;
//    primaryFlare.size = 0.3f;
//    primaryFlare.color = glm::vec3(1.0f, 0.9f, 0.7f);  // Warm white
//    primaryFlare.alpha = 0.7f;
//    flares.push_back(primaryFlare);
//
//    // Add secondary flares along the line from sun to screen center
//    float spacings[] = { 0.2f, 0.4f, 0.6f, 0.8f, 1.2f, 1.5f };
//    float sizes[] = { 0.05f, 0.1f, 0.07f, 0.15f, 0.04f, 0.12f };
//
//    glm::vec3 colors[] = {
//        glm::vec3(1.0f, 0.8f, 0.4f),  // Golden
//        glm::vec3(0.9f, 0.5f, 0.2f),  // Orange
//        glm::vec3(0.6f, 0.8f, 1.0f),  // Blue
//        glm::vec3(0.7f, 1.0f, 0.8f),  // Cyan-ish
//        glm::vec3(1.0f, 0.6f, 0.6f),  // Pink
//        glm::vec3(0.9f, 0.9f, 1.0f)   // Light purple
//    };
//
//    for (int i = 0; i < 6; i++) {
//        LensFlare flare;
//        flare.position = sunNDC + direction * spacings[i] * distToCenter;
//        flare.size = sizes[i];
//        flare.color = colors[i];
//        flare.alpha = 0.3f + 0.3f * (1.0f - (float)i / 6.0f);  // Fade out distant flares
//        flares.push_back(flare);
//    }
//
//    // Add a few random smaller flares
//    for (int i = 0; i < 3; i++) {
//        float randomOffset = (float)(rand() % 100) / 100.0f * 0.1f;
//        float pos = 0.3f + i * 0.25f;
//
//        LensFlare flare;
//        flare.position = sunNDC + direction * (pos + randomOffset) * distToCenter;
//        flare.size = 0.02f + (rand() % 100) / 100.0f * 0.04f;
//        flare.color = glm::vec3(1.0f, 0.9f, 0.8f);
//        flare.alpha = 0.2f;
//        flares.push_back(flare);
//    }
//
//    return flares;
//}
//
//void generateOrbitLines(const std::vector<Planet>& planets, int segments = 100) {
//    orbitLineVertices.clear();
//
//    for (const auto& planet : planets) {
//        if (planet.name == "Sun") continue;  // Skip the sun
//
//        std::vector<float> planetOrbit;
//        for (int i = 0; i <= segments; ++i) {
//            float angle = 2.0f * M_PI * float(i) / float(segments);
//            float x = planet.distance * cos(angle);
//            float z = planet.distance * sin(angle);
//
//            // Store just position for orbit lines
//            planetOrbit.push_back(x);
//            planetOrbit.push_back(0.0f);  // y is always 0 for orbit plane
//            planetOrbit.push_back(z);
//        }
//        orbitLineVertices.push_back(planetOrbit);
//    }
//    // Setup VAO/VBO for orbit lines
//    glBindVertexArray(orbitLinesVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, orbitLinesVBO);
//
//    // Calculate total size needed for all orbits
//    size_t totalSize = 0;
//    for (const auto& orbit : orbitLineVertices) {
//        totalSize += orbit.size() * sizeof(float);
//    }
//
//    // Allocate buffer of appropriate size
//    glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);
//
//    // Set up vertex attributes for orbit lines
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//}
//
//void loadOBJ(const std::string& path, std::vector<float>& vertices) {
//    std::ifstream file(path);
//    std::vector<glm::vec3> temp_vertices;
//    std::vector<glm::vec3> temp_normals;
//    std::vector<unsigned int> vertexIndices, normalIndices;
//
//    std::string line;
//    while (std::getline(file, line)) {
//        std::istringstream iss(line);
//        std::string type;
//        iss >> type;
//        if (type == "v") {
//            glm::vec3 vertex;
//            iss >> vertex.x >> vertex.y >> vertex.z;
//            temp_vertices.push_back(vertex);
//        }
//        else if (type == "vn") {
//            glm::vec3 normal;
//            iss >> normal.x >> normal.y >> normal.z;
//            temp_normals.push_back(normal);
//        }
//        else if (type == "f") {
//            for (int i = 0; i < 3; i++) {
//                std::string vert;
//                iss >> vert;
//                size_t pos1 = vert.find("/");
//                size_t pos2 = vert.rfind("/");
//                int vi = std::stoi(vert.substr(0, pos1)) - 1;
//                int ni = std::stoi(vert.substr(pos2 + 1)) - 1;
//                glm::vec3 v = temp_vertices[vi];
//                glm::vec3 n = temp_normals[ni];
//                vertices.insert(vertices.end(), { v.x, v.y, v.z, n.x, n.y, n.z });
//            }
//        }
//    }
//}
//
//vector<float> shipVertices;
//GLuint shipVAO, shipVBO;
//GLuint shipShaderProgram;
//
//void setupShip() {
//    glGenVertexArrays(1, &shipVAO);
//    glGenBuffers(1, &shipVBO);
//    glBindVertexArray(shipVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, shipVBO);
//    glBufferData(GL_ARRAY_BUFFER, shipVertices.size() * sizeof(float), shipVertices.data(), GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    GLuint shipVertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(shipVertexShader, 1, &shipVertexShaderSource, NULL);
//    glCompileShader(shipVertexShader);
//    GLuint shipFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(shipFragmentShader, 1, &shipFragmentShaderSource, NULL);
//    glCompileShader(shipFragmentShader);
//    shipShaderProgram = glCreateProgram();
//    glAttachShader(shipShaderProgram, shipVertexShader);
//    glAttachShader(shipShaderProgram, shipFragmentShader);
//    glLinkProgram(shipShaderProgram);
//    glDeleteShader(shipVertexShader);
//    glDeleteShader(shipFragmentShader);
//
//    glEnable(GL_DEPTH_TEST);
//}
//
//int main() {
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    generateSphereMesh(vertices, indices);
//
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
//    if (!window) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate(); return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//    glfwSetMouseButtonCallback(window, mouse_button_callback);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cerr << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//
//    ImFont* defaultFont = io.Fonts->AddFontFromFileTTF("fonts/Poppins-Medium.ttf", 25.0f);
//    ImFont* boldFont = io.Fonts->AddFontFromFileTTF("fonts/Poppins-Bold.ttf", 29.0f);
//    ImFont* logoFont = io.Fonts->AddFontFromFileTTF("fonts/Poppins-Bold.ttf", 35.0f);
//
//    ImGui::StyleColorsDark();
//    ImGuiStyle& style = ImGui::GetStyle();
//
//    ImVec4* colors = style.Colors;
//    colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);  
//    colors[ImGuiCol_Border] = ImVec4(0.09f, 0.09f, 0.09f, 0.54f);
//    colors[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);   
//    colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
//    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
//    colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.36f, 1.00f);
//    colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
//    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.26f, 0.30f, 1.00f);
//    colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.28f, 0.32f, 1.00f);
//    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
//    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
//    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
//    colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
//    colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.33f, 1.00f);
//    colors[ImGuiCol_TabHovered] = ImVec4(0.23f, 0.23f, 0.29f, 1.00f);
//
//    // Add rounded corners
//    style.WindowRounding = 15.0f;      // Keep main windows square
//    style.ChildRounding = 15.0f;       // Round panels
//    style.FrameRounding = 8.0f;       // Round buttons and input fields
//    style.PopupRounding = 4.0f;
//    style.ScrollbarRounding = 4.0f;
//    style.TabRounding = 4.0f;
//    style.WindowPadding = ImVec2(18, 18);
//    style.ItemSpacing = ImVec2(18, 10);
//    style.FramePadding = ImVec2(6, 6);
//    style.IndentSpacing = 22.0f;
//
//    // Borders
//    style.WindowBorderSize = 0.0f;    // No border for main windows
//    style.ChildBorderSize = 1.0f;
//
//    // Setup Platform/Renderer bindings
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 130");
//
//    //Para sa Vertices and Edges of Planets (Spheres)
//    GLuint VBO, VAO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    // Create a special shader program for the sun
//    GLuint sunFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(sunFragmentShader, 1, &sunFragmentShaderSource, NULL);
//    glCompileShader(sunFragmentShader);
//
//    GLuint sunShaderProgram = glCreateProgram();
//    glAttachShader(sunShaderProgram, vertexShader); 
//    glAttachShader(sunShaderProgram, sunFragmentShader);
//    glLinkProgram(sunShaderProgram);
//    glDeleteShader(sunFragmentShader);
//
//    vector<Planet> planets = {
//        {"Mercury",  50.0f, 0.5f,  47.4f, 1.0f, 0.0f, loadTexture("textures/mercury.jpg")},
//        {"Venus",   100.0f, 0.95f, 35.0f, 1.0f, 2.57f, loadTexture("textures/venus.jpg")},
//        {"Earth",   175.0f, 1.0f,  29.8f, 1.0f, 4.2f, loadTexture("textures/earth.jpg")},
//        {"Mars",    205.0f, 0.53f, 24.1f, 1.0f, 7.9f, loadTexture("textures/mars.jpg")},
//        {"Jupiter", 288.0f, 5.0f,  13.1f, 1.0f, 8.5f, loadTexture("textures/jupiter.jpg")},
//        {"Saturn",  338.0f, 7.5f,   9.7f, 1.0f, 10.4f,  loadTexture("textures/saturn.jpg")},
//        {"Sun",     0.0f, 10.5f,  1.0f, 1.0f, 0.0f,   loadTexture("textures/sun.jpg")}
//    };
//
//
//    vector<float> ringVertices;
//    vector<unsigned int> ringIndices;
//    generateRingMesh(ringVertices, ringIndices, 1.2f, 2.0f); 
//
//    GLuint ringVAO, ringVBO, ringEBO;
//    glGenVertexArrays(1, &ringVAO);
//    glGenBuffers(1, &ringVBO);
//    glGenBuffers(1, &ringEBO);
//
//    glBindVertexArray(ringVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, ringVBO);
//    glBufferData(GL_ARRAY_BUFFER, ringVertices.size() * sizeof(float), ringVertices.data(), GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ringEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ringIndices.size() * sizeof(unsigned int), ringIndices.data(), GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    GLuint ringTextureID = loadTexture("textures/saturn_ring.jpg");
//    unsigned int ringIndexCount = static_cast<unsigned int>(ringIndices.size());
//
//    setupLensFlare();
//
//    generateOrbitLines(planets);
//
//    glGenVertexArrays(1, &orbitLinesVAO);
//    glGenBuffers(1, &orbitLinesVBO);
//
//    loadOBJ("models/Ship_Endurance.obj", shipVertices);
//    setupShip();
//    
//    setupGalaxy();
//    GLuint galaxyVertexShader = glCreateShader(GL_VERTEX_SHADER);
//
//    glShaderSource(galaxyVertexShader, 1, &galaxyVertexShaderSource, NULL);
//    glCompileShader(galaxyVertexShader);
//
//    GLuint galaxyFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//
//    glShaderSource(galaxyFragmentShader, 1, &galaxyFragmentShaderSource, NULL);
//    glCompileShader(galaxyFragmentShader);
//
//    GLuint galaxyShaderProgram = glCreateProgram();
//    glAttachShader(galaxyShaderProgram, galaxyVertexShader);
//    glAttachShader(galaxyShaderProgram, galaxyFragmentShader);
//    glLinkProgram(galaxyShaderProgram);
//
//    glDeleteShader(galaxyVertexShader);
//    glDeleteShader(galaxyFragmentShader);
//
//    while (!glfwWindowShouldClose(window)) {
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        bool imguiWantsCapture = ImGui::GetIO().WantCaptureMouse;
//
//
//        float currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        processInput(window);
//
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 50000.0f);
//        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//        glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f); // Sun at center
//
//        // FIRST: Render the sun with special shader
//        glUseProgram(sunShaderProgram);
//
//        // Pass uniforms to sun shader
//        glUniformMatrix4fv(glGetUniformLocation(sunShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//        glUniformMatrix4fv(glGetUniformLocation(sunShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
//        glUniform1f(glGetUniformLocation(sunShaderProgram, "time"), currentFrame);
//
//        // Find and render the sun
//        for (const auto& planet : planets) {
//            if (planet.name == "Sun") {
//                glBindVertexArray(VAO);
//                glm::mat4 model = glm::mat4(1.0f);
//                model = glm::translate(model, glm::vec3(planet.distance, 0.0f, 0.0f));
//                model = glm::scale(model, glm::vec3(planet.size));
//                glUniformMatrix4fv(glGetUniformLocation(sunShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//                glBindTexture(GL_TEXTURE_2D, planet.textureID);
//                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//
//                // Add glow effect with additive blending
//                glEnable(GL_BLEND);
//                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//
//                // Slightly larger sphere for the glow
//                model = glm::mat4(1.0f);
//                model = glm::translate(model, glm::vec3(planet.distance, 0.0f, 0.0f));
//                model = glm::scale(model, glm::vec3(planet.size * 1.05f));
//                glUniformMatrix4fv(glGetUniformLocation(sunShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//
//                // Restore default blending
//                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//                glDisable(GL_BLEND);
//                break;
//            }
//        }
//
//        //Galaxy
//
//
//        glUseProgram(galaxyShaderProgram);
//
//        glm::mat4 galaxyModel = glm::mat4(1.0f);
//
//        // Apply a FIXED tilt (e.g., 45 degrees) around X axis
//        galaxyModel = glm::rotate(galaxyModel, glm::radians(-90.0f), glm::vec3(1, 0, 0));
//
//        // Then apply a TIME-BASED rotation around Z axis
//        galaxyModel = glm::rotate(galaxyModel, -timeFactor * 0.08f, glm::vec3(0, 0, 1));
//
//        galaxyModel = glm::translate(galaxyModel, glm::vec3(1.0f, 0.0f, 1.0f));
//        galaxyModel = glm::scale(galaxyModel, glm::vec3(80000.0f));
//
//
//        glUniformMatrix4fv(glGetUniformLocation(galaxyShaderProgram, "model"), 1, GL_FALSE, &galaxyModel[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(galaxyShaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(galaxyShaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
//
//        glBindVertexArray(galaxyVAO);
//        glDrawArrays(GL_POINTS, 0, (GLsizei)particles.size());
//
//
//        if (showOrbitLines) {
//            glUseProgram(shaderProgram);  // Use the same shader as planets
//
//            // Set line properties
//            glLineWidth(1.0f);
//            glEnable(GL_LINE_SMOOTH);
//
//            // Bind orbit lines VAO
//            glBindVertexArray(orbitLinesVAO);
//
//            for (size_t i = 0; i < orbitLineVertices.size(); ++i) {
//                // Skip orbit for sun
//                if (planets[i].name == "Sun") continue;
//
//                // Set up model matrix (identity for orbits as they're already at world space)
//                glm::mat4 model = glm::mat4(1.0f);
//                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//
//                // Update VBO with this planet's orbit
//                glBindBuffer(GL_ARRAY_BUFFER, orbitLinesVBO);
//                glBufferData(GL_ARRAY_BUFFER, orbitLineVertices[i].size() * sizeof(float),
//                    orbitLineVertices[i].data(), GL_STATIC_DRAW);
//
//                // Set vertex attributes
//                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//                glEnableVertexAttribArray(0);
//
//                // Draw the orbit
//                glDrawArrays(GL_LINE_LOOP, 0, orbitLineVertices[i].size() / 3);
//            }
//            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        }
//
//        glUseProgram(shipShaderProgram);
//
//        glm::mat4 shipModel = glm::mat4(1.0f);
//        glm::vec3 initialPosition = glm::vec3(-30.0f, 0.0f, 100.0f);
//
//        glm::vec3 shipTrajectory = glm::vec3(0.0f, 0.0f, 1.0f);
//        float shipSpeed = 50000.0f * 2.0f;
//
//        glm::vec3 shipPosition = initialPosition + shipTrajectory * currentFrame * shipSpeed * timeFactor;
//        shipModel = glm::translate(shipModel, shipPosition);
//
//        float spinSpeed = glm::radians(900000.0f) * 10.0f;
//        float spinAngle = currentFrame * spinSpeed * timeFactor;
//        shipModel = glm::rotate(shipModel, spinAngle, glm::vec3(1.0f, 0.0f, 0.0f));
//
//        shipModel = glm::scale(shipModel, glm::vec3(0.00003f));
//
//        glUniformMatrix4fv(glGetUniformLocation(shipShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(shipModel));
//        glUniformMatrix4fv(glGetUniformLocation(shipShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
//        glUniformMatrix4fv(glGetUniformLocation(shipShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//        glUniform3fv(glGetUniformLocation(shipShaderProgram, "lightPos"), 1, glm::value_ptr(glm::vec3(5.0f, 5.0f, 5.0f)));
//        glUniform3fv(glGetUniformLocation(shipShaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
//
//        glBindVertexArray(shipVAO);
//        glDrawArrays(GL_TRIANGLES, 0, shipVertices.size() / 6);
//
//        //Render yung planets after the ship and sun
//        glUseProgram(shaderProgram);
//        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
//        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
//        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
//
//        for (const auto& planet : planets) {
//            if (planet.name == "Sun") continue;
//
//            glBindVertexArray(VAO);
//            float angle = planet.initialAngle + currentFrame * planet.orbitSpeed * timeFactor;
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
//            model = glm::translate(model, glm::vec3(planet.distance, 0.0f, 0.0f));
//            model = glm::scale(model, glm::vec3(planet.size));
//            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//            glBindTexture(GL_TEXTURE_2D, planet.textureID);
//            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//
//            if (planet.name == "Saturn") {
//                glm::mat4 ringModel = model;
//                ringModel = glm::scale(ringModel, glm::vec3(2.5f, 1.0f, 2.5f));
//                ringModel = glm::rotate(ringModel, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//
//                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(ringModel));
//
//                glActiveTexture(GL_TEXTURE0);
//                glBindTexture(GL_TEXTURE_2D, ringTextureID);
//                glBindVertexArray(ringVAO);
//                glDrawElements(GL_TRIANGLES, ringIndexCount, GL_UNSIGNED_INT, 0);
//            }
//        }
//
//        glm::vec3 sunWorldPos(0.0f);
//        for (const auto& planet : planets) {
//            if (planet.name == "Sun") {
//                sunWorldPos = glm::vec3(planet.distance, 0.0f, 0.0f);
//                break;
//            }
//        }
//
//
//
//        // Calculate and render lens flares
//        std::vector<LensFlare> flares = calculateLensFlares(sunWorldPos, view, projection);
//
//        // Enable blending for flares
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//        glDisable(GL_DEPTH_TEST);  // Disable depth testing for flares
//
//        // Render each flare
//        for (const auto& flare : flares) {
//            renderLensFlare(flare);
//        }
//
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glEnable(GL_DEPTH_TEST);
//
//        ShowPlanetDashboard();
//        ShowPlanetStatisticsWithGraph(timeFactor);
//
//        ImGui::Render();
//
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glDeleteVertexArrays(1, &orbitLinesVAO);
//    glDeleteBuffers(1, &orbitLinesVBO);
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//
//    glfwTerminate();
//    return 0;
//}
//
//
