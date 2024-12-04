#pragma once
#include <GL/glew.h>
#include <string>

class Present2 {
public:
    Present2(); 
    ~Present2();
    void render();

private:
    unsigned int VAO, VBO, textureID, shaderProgram;
};