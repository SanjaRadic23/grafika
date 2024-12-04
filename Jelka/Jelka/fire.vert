#version 330 core
layout(location = 0) in vec2 inPos;     // Vertex position
layout(location = 1) in vec2 inTex;     // Texture coordinates
out vec2 chTex;                         // Pass texture coordinates
out vec2 fragPos;                       // Pass fragment position
void main() {
    gl_Position = vec4(inPos, 0.0, 1.0);  // Simple transformation
    fragPos = inPos;                      // Store position for lighting effect
    chTex = inTex;                        // Pass texture coordinates
}