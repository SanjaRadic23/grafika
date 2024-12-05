#version 330 core

layout(location = 0) in vec2 inPos; //pozicija tacke, x,y
layout(location = 1) in vec3 inCol; //rgb u ovom slucaju, cesto su to s,t za teksturu

out vec2 fragCoord; //koordinate za frag
out vec3 chCol; //boja frag

void main() {
    //gl_Position ce da prilagodi klipovanom prozoru, iz model u kamera space, 2d u 4d (homogenizacija)
    gl_Position = vec4(inPos, 0.0, 1.0);
    gl_PointSize = 20.0; //ovo glEnable(GL_PROGRAM_POINT_SIZE); si dodala u main da bi radilo
    fragCoord = (inPos + 1.0) * 0.5; //normalizovanje od 0.0 do 1.0, mi stavljamo od -1 do 1
    chCol = inCol; //samo prosledjujemo rgb
}