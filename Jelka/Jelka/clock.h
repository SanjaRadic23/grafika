#pragma once
#include <GL/glew.h>
#include <string>

class Clock {
public:
    Clock();
    ~Clock();
    void render();

private:
    unsigned int VAO, VBO, textureID, shaderProgram; 
};