#version 330 core
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inCol;

out vec2 fragCoord;
out vec3 chCol;

void main() {
    gl_Position = vec4(inPos, 0.0, 1.0);
    gl_PointSize = 20.0; //ovo glEnable(GL_PROGRAM_POINT_SIZE); si dodala u main da bi radilo
    fragCoord = (inPos + 1.0) * 0.5;
    chCol = inCol;
}