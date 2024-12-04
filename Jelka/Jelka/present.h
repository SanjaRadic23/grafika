#pragma once
#include <GL/glew.h>
#include <string>

class Present {
public:
    Present(); 
    ~Present();
    void render();

private:
    unsigned int VAO, VBO, textureID, shaderProgram;
};