#pragma once
#include <GL/glew.h>
#include <string>

class Shelf {
public:
    Shelf();
    ~Shelf();
    void render(); 

private:
    unsigned int VAO, VBO, textureID, shaderProgram;
};