#version 330 core

layout(location = 0) in vec4 vertex; // (x, y, s, t)
out vec2 TexCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0); //da se dobije polozaj verteksa u clip spaceu
    TexCoords = vertex.zw; //samo s i t saljemo frag
}