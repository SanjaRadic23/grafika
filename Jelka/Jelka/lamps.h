#pragma once
#include <GL/glew.h>
#include <vector>
#include <cmath>

class Lamps {
public:
    Lamps();
    ~Lamps();
    void render(float time, int mode, float lampRadius);

private:
    unsigned int VAO, VBO, shaderProgram;
};