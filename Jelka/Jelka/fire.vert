#version 330 core

layout(location = 0) in vec2 inPos; // Pozicija verteksa
layout(location = 1) in vec2 inTex; // Teksturne koordinate

out vec2 chTex; // Prosle?ujemo teksturne koordinate
out vec2 fragPos; // Prosle?ujemo poziciju fragmentu

void main() {
    gl_Position = vec4(inPos, 0.0, 1.0); // Prosta transformacija
    fragPos = inPos; // ?uvamo poziciju za efekat osvetljenja
    chTex = inTex; // Prosle?ujemo teksturne koordinate
}
