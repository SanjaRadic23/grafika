#pragma once
#include <GL/glew.h>
#include <vector>
#include <cmath>

class Lamps {
public:
    Lamps(); //konstruktor za inicijalizovanje
    ~Lamps(); //destruktor (unistavanje i oslobadjanje)
    void render(float time, int mode, float lampRadius); //mode 1 ili 2 za nacin gorenja lampica, render za prikaz
private:
    unsigned int VAO, VBO, shaderProgram; //neophodne promenljive za rad sa OpenGL-om
};