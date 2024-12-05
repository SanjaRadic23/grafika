#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
    float sampled = texture(text, TexCoords).r; //samo crvenu koristimo od boja jer je grey_scale
    color = vec4(textColor, sampled); //intenzitet da bude crven, tamnije i svetlije crno u grey_scale
}